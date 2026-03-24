/**
  * @file md5.h
  * @brief MD5 (Message-Digest Algorithm)
  *
  * @section License
  *
  * SPDX-License-Identifier: GPL-2.0-or-later
  *
  * Copyright (C) 2010-2022 Oryx Embedded SARL. All rights reserved.
  *
  * This file is part of CycloneCRYPTO Open.
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License
  * as published by the Free Software Foundation; either version 2
  * of the License, or (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful,
  * but WITHOUT ANY WARRANTY; without even the implied warranty of
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  * GNU General Public License for more details.
  *
  * You should have received a copy of the GNU General Public License
  * along with this program; if not, write to the Free Software Foundation,
  * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
  *
  * @author Oryx Embedded SARL (www.oryx-embedded.com)
  * @version 2.1.8
  **/

#ifndef _MD5_H
#define _MD5_H

#include <cstdint>
#include <string>
#include <sstream>
#include <iomanip>

namespace Hash {
	class MD5 {
		/**
		 * MD5 algorithm context
		 **/
		typedef struct
		{
			union {
				uint32_t h[4];
				uint8_t digest[16];
			};
			union {
				uint32_t x[16];
				uint8_t buffer[64];
			};
			size_t size;
			uint64_t totalSize;
		} Md5Context;
		Md5Context dataContext;

		//MD5 related functions
		long md5Compute(const void* data, size_t length, uint8_t* digest);
		void md5Init(Md5Context* context);
		void md5Update(Md5Context* context, const void* data, size_t length);
		void md5Final(Md5Context* context, uint8_t* digest);
		void md5FinalRaw(Md5Context* context, uint8_t* digest);
		void md5ProcessBlock(Md5Context* context);
	public:
		MD5()
		{
			md5Init(&dataContext);
		}
		void Reset() {
			md5Init(&dataContext);
		}
		void Update(const void* data, size_t length) {
			md5Update(&dataContext, data, length);
		}
		std::string Finalize() {
			uint8_t digest[16];
			md5Final(&dataContext, digest);
			std::stringstream ss;

			for (int i = 0; i < 16; ++i)
				ss << std::hex
				<< std::setw(2)
				<< std::setfill('0')
				<< (int)digest[(size_t)i];

			return ss.str();
		}
	};
}
#endif
