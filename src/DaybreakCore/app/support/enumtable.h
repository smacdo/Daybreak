/*
 * Copyright 2019 Scott MacDonald
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once
#include <string>

namespace Daybreak
{
    template<typename TType>
    struct enum_entry_t
    {
        TType id;
        const char* name;
        const char* description;
        const wchar_t* wideName;
        const wchar_t* wideDescription;
    };
}

#define DAYBREAK_DEFINE_ENUM_NAMES(tableName, id) static const enum_entry_t<typename id> LUT_##tableName[] {
#define DAYBREAK_ENUM(namedValue) { (namedValue), #namedValue, "", L#namedValue, L"" },
#define DAYBREAK_ENUM_WITH_DESC(namedValue, description) { (namedValue), #namedValue, description, L#namedValue, L##description },
#define DAYBREAK_END_ENUM_NAMES(tableName) \
    }; \
    DAYBREAK_CREATE_ENUM_NAME_FUNC(tableName, tableName) \
    

#define DAYBREAK_CREATE_ENUM_NAME_FUNC(tableName, EType) \
	const char * tableName##Name(EType value) noexcept { \
		auto count = sizeof(LUT_##tableName) / sizeof(LUT_##tableName[0]); \
		for (size_t i = 0; i < count; ++i) { \
			if (LUT_##tableName[i].id == value) { return LUT_##tableName[i].name; } \
			} \
		return "ENUM_NAME_NOT_FOUND"; \
	} \
	const wchar_t * tableName##WideName(EType value) noexcept { \
		auto count = sizeof(LUT_##tableName) / sizeof(LUT_##tableName[0]); \
		for (size_t i = 0; i < count; ++i) { \
			if (LUT_##tableName[i].id == value) { return LUT_##tableName[i].wideName; } \
			} \
		return L"ENUM_NAME_NOT_FOUND"; \
	} \
	const char * tableName##Description(EType value) noexcept { \
		auto count = sizeof(LUT_##tableName) / sizeof(LUT_##tableName[0]); \
		for (size_t i = 0; i < count; ++i) { \
			if (LUT_##tableName[i].id == value) { return LUT_##tableName[i].description; } \
			} \
		return "ENUM_DESCRIPTION_NOT_FOUND"; \
	} \
	const wchar_t * tableName##WideDescription(EType value) noexcept { \
		auto count = sizeof(LUT_##tableName) / sizeof(LUT_##tableName[0]); \
		for (size_t i = 0; i < count; ++i) { \
			if (LUT_##tableName[i].id == value) { return LUT_##tableName[i].wideDescription; } \
			} \
		return L"ENUM_DESCRIPTION_NOT_FOUND"; \
	} \
