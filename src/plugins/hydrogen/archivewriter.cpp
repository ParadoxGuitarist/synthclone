/*
 * libsynthclone_hydrogen - Hydrogen target plugin for `synthclone`
 * Copyright (C) 2011-2013 Devin Anderson
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 675 Mass
 * Ave, Cambridge, MA 02139, USA.
 */

#include <cassert>
#include <stdexcept>

#include <archive_entry.h>

#include <QtCore/QDebug>

#include <synthclone/error.h>

#include "archivewriter.h"

ArchiveWriter::ArchiveWriter(const QString &path, QObject *parent):
    QObject(parent)
{
    arch = archive_write_new();
    if (! arch) {
        throw std::bad_alloc();
    }
    try {
        if (archive_write_set_compression_gzip(arch) != ARCHIVE_OK) {
            throw synthclone::Error(archive_error_string(arch));
        }
        if (archive_write_set_format_pax_restricted(arch) != ARCHIVE_OK) {
            throw synthclone::Error(archive_error_string(arch));
        }
        QByteArray pathBytes = path.toLocal8Bit();
        if (archive_write_open_filename(arch, pathBytes.constData()) !=
            ARCHIVE_OK) {
            throw synthclone::Error(archive_error_string(arch));
        }
    } catch (...) {
        archive_write_finish(arch);
        throw;
    }
    closed = false;
}

ArchiveWriter::~ArchiveWriter()
{
    if (! closed) {
        try {
            close();
        } catch (synthclone::Error &e) {
            qWarning() << tr("Error in ArchiveWriter destructor: %1").
                arg(e.getMessage());
        }
    }
    archive_write_finish(arch);
}

void
ArchiveWriter::close()
{
    assert(! closed);
    if (archive_write_close(arch) != ARCHIVE_OK) {
        throw synthclone::Error(archive_error_string(arch));
    }
    closed = true;
}

void
ArchiveWriter::writeData(const QByteArray &data)
{
    ssize_t size = static_cast<ssize_t>(data.count());
    ssize_t n = archive_write_data(arch, data.constData(), size);
    if (n == -1) {
        throw synthclone::Error(archive_error_string(arch));
    }
    assert(n == size);
}

void
ArchiveWriter::writeHeader(const ArchiveHeader &header)
{
    struct archive_entry *entry = archive_entry_new();
    if (! entry) {
        throw std::bad_alloc();
    }
    QByteArray path = header.getPath().toLocal8Bit();
    qint64 size = header.getSize();
    archive_entry_set_pathname(entry, path.constData());
    archive_entry_set_size(entry, static_cast<int64_t>(size));
    archive_entry_set_filetype(entry, AE_IFREG);
    archive_entry_set_perm(entry, 0644);
    int result = archive_write_header(arch, entry);
    archive_entry_free(entry);
    if (result != ARCHIVE_OK) {
        throw synthclone::Error(archive_error_string(arch));
    }
}
