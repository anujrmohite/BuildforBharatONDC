#ifndef OSMIUM_IO_READER_WITH_PROGRESS_BAR_HPP
#define OSMIUM_IO_READER_WITH_PROGRESS_BAR_HPP

/*

This file is part of Osmium (https://osmcode.org/libosmium).

Copyright 2013-2022 Jochen Topf <jochen@topf.org> and others (see README).

Boost Software License - Version 1.0 - August 17th, 2003

Permission is hereby granted, free of charge, to any person or organization
obtaining a copy of the software and accompanying documentation covered by
this license (the "Software") to use, reproduce, display, distribute,
execute, and transmit the Software, and to prepare derivative works of the
Software, and to permit third-parties to whom the Software is furnished to
do so, all subject to the following:

The copyright notices in the Software and this entire statement, including
the above license grant, this restriction and the following disclaimer,
must be included in all copies of the Software, in whole or in part, and
all derivative works of the Software, unless such copies or derivative
works are solely in the form of machine-executable object code generated by
a source language processor.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.

*/

#include <osmium/io/input_iterator.hpp>
#include <osmium/io/reader.hpp>
#include <osmium/memory/buffer.hpp>
#include <osmium/util/progress_bar.hpp>

namespace osmium {

    namespace io {

        /**
         * Wraps an osmium::io::Reader and an osmium::ProgressBar into
         * one handy package.
         */
        class ReaderWithProgressBar : public Reader {

            ProgressBar m_progress_bar;

        public:

            /**
             * Constructor.
             *
             * @param enable Enable flag for the progress bar.
             *
             * All other parameters are forwarded to the Reader.
             */
            template <typename... TArgs>
            explicit ReaderWithProgressBar(bool enable, TArgs&&... args) :
                Reader(std::forward<TArgs>(args)...),
                m_progress_bar(file_size(), enable) {
            }

            /**
             * Read a buffer from the Reader updating the progress bar in
             * the process.
             */
            osmium::memory::Buffer read() {
                auto buffer = Reader::read();
                if (buffer) {
                    m_progress_bar.update(offset());
                } else {
                    m_progress_bar.done();
                }
                return buffer;
            }

            /// Get the underlying ProgressBar object.
            ProgressBar& progress_bar() noexcept {
                return m_progress_bar;
            }

        }; // class ReaderWithProgressBar

        inline InputIterator<ReaderWithProgressBar> begin(ReaderWithProgressBar& reader) {
            return InputIterator<ReaderWithProgressBar>(reader);
        }

        inline InputIterator<ReaderWithProgressBar> end(ReaderWithProgressBar& /*reader*/) {
            return InputIterator<ReaderWithProgressBar>();
        }

    } // namespace io

} // namespace osmium

#endif // OSMIUM_IO_READER_WITH_PROGRESS_BAR_HPP
