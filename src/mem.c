/*
 * HeliOS Embedded Operating System
 * Copyright (C) 2020 Manny Peterson <me@mannypeterson.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "HeliOS.h"
#include "list.h"
#include "mem.h"
#include "task.h"
#include "timer.h"

volatile struct MemAllocRecord memAllocTable[MEMALLOCTABLESIZE];

void MemInit() {
  memset_(&memAllocTable, 0, MEMALLOCTABLESIZE * sizeof(struct MemAllocRecord));
}

void* xMemAlloc(size_t size_) {
  void* ptr = NULL;
  if(size_ > 0) {
    for (int i = 0; i < MEMALLOCTABLESIZE; i++) {
      if (!memAllocTable[i].ptr) {
        ptr = calloc(1, size_);
        if (ptr) {
          memAllocTable[i].size = size_;
          memAllocTable[i].ptr = ptr;
          return ptr;
        }
      }
    }
  }
  return NULL;
}

void xMemFree(void* ptr_) {
  if (ptr_) {
    for (int i = 0; i < MEMALLOCTABLESIZE; i++) {
      if (memAllocTable[i].ptr == ptr_) {
        free(memAllocTable[i].ptr);
        memAllocTable[i].size = 0;
        memAllocTable[i].ptr = NULL;
      }
    }
  }
}

int xMemGetUsed() {
  int used = 0;
  for (int i = 0; i < MEMALLOCTABLESIZE; i++) {
    if (memAllocTable[i].ptr) {
      used += (int) memAllocTable[i].size;
    }
  }
  return used;
}

int xMemGetSize(void* ptr_) {
  if (ptr_) {
    for (int i = 0; i < MEMALLOCTABLESIZE; i++) {
      if (memAllocTable[i].ptr == ptr_) {
        return memAllocTable[i].size;
      }
    }
  }
  return 0;
}

void MemClear() {
  for (int i = 0; i < MEMALLOCTABLESIZE; i++) {
    if (memAllocTable[i].ptr) {
      free(memAllocTable[i].ptr);
      memAllocTable[i].size = 0;
      memAllocTable[i].ptr = NULL;
    }
  }
}
