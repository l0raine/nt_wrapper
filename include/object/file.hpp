/*
 * Copyright 2018 Justas Masiulis
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once
#include "handle.hpp"

namespace ntw::obj {
    namespace detail {

        template<class Handle>
        class basic_file {
            Handle _handle;

            NT_FN _open(UNICODE_STRING path,
                        bool           create_if_not_existing,
                        unsigned long  access_flags,
                        unsigned long  create_flags) noexcept;

        public:
            class disposer {
                std::wstring_view _path;

            public:
                NTW_INLINE disposer(std::wstring_view file_path) : _path(file_path) {}
                NTW_INLINE ~disposer() { static_cast<void>(unique_file::destroy(_path)); }
            };

            NTW_INLINE basic_file()  = default;
            NTW_INLINE ~basic_file() = default;
            NTW_INLINE basic_file(void* handle) : _handle(handle) {}

            NTW_INLINE Handle& handle() noexcept { return _handle; }
            NTW_INLINE const Handle& handle() const noexcept { return _handle; }


            template<class StringRef /* wstring_view or UNICODE_STRING */>
            NT_FN open_dir(const StringRef& path,
                           bool             create_if_not_existing = false,
                           unsigned long access_flags = FILE_READ_DATA | FILE_WRITE_DATA |
                                                        DELETE) noexcept;

            template<class StringRef /* wstring_view or UNICODE_STRING */>
            NT_FN open(const StringRef& path,
                       bool             create_if_not_existing = false,
                       unsigned long    access_flags = FILE_READ_DATA | FILE_WRITE_DATA |
                                                    DELETE) noexcept;

            NT_FN open_as_pipe(UNICODE_STRING name,
                               unsigned long  in_max,
                               unsigned long  out_max,
                               unsigned long  instances = -1,
                               LARGE_INTEGER  timeout = make_large_int(-500000)) noexcept;

            NT_FN size(std::uint64_t& size_out) const noexcept;

            NT_FN write(const void*    data,
                        unsigned long  size,
                        unsigned long* written = nullptr) const noexcept;

            NT_FN
            read(void* buffer, unsigned long size, unsigned long* read = nullptr) const
                noexcept;

            template<class Callback, class... Args>
            NT_FN enum_contained_files(Callback callback, Args&&... args) const noexcept;

            template<class StringRef /* wstring_view or UNICODE_STRING */>
            NT_FN static destroy(const StringRef& path,
                                 bool             case_sensitive = false) noexcept;
        };

    } // namespace detail

    using unique_file = detail::basic_file<unique_handle>;
    using file_ref    = detail::basic_file<handle_ref>;

} // namespace ntw::obj

#include "../impl/file.inl"