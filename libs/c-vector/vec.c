/*
BSD 3-Clause License

Copyright (c) 2024, Mashpoe
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "vec.h"
#include <string.h>

typedef struct vector_data vector_data;

struct vector_data
{
	vec_size_t alloc; // stores the number of bytes allocated
	vec_size_t length;
	char buff[]; // use char to store bytes of an unknown type
};

vector_data* vector_alloc(vec_size_t alloc, vec_size_t size)
{
	vector_data* v_data = malloc(sizeof(vector_data) + alloc * size);
	v_data->alloc = alloc;
	return v_data;
}

vector_data* vector_get_data(vector vec) { return &((vector_data*)vec)[-1]; }

vector vector_create(void)
{
	vector_data* v = malloc(sizeof(vector_data));
	v->alloc = 0;
	v->length = 0;

	return &v->buff;
}

void vector_free(vector vec) { free(vector_get_data(vec)); }

vec_size_t vector_size(vector vec) { return vector_get_data(vec)->length; }

vec_size_t vector_get_alloc(vector vec) { return vector_get_data(vec)->alloc; }

vector_data* vector_realloc(vector_data* v_data, vec_type_t type_size)
{
	vec_size_t new_alloc = (v_data->alloc == 0) ? 1 : v_data->alloc * 2;
	vector_data* new_v_data =
	    realloc(v_data, sizeof(vector_data) + new_alloc * type_size);
	new_v_data->alloc = new_alloc;
	return new_v_data;
}

bool vector_has_space(vector_data* v_data)
{
	return v_data->alloc - v_data->length > 0;
}

void* _vector_add(vector* vec_addr, vec_type_t type_size)
{
	vector_data* v_data = vector_get_data(*vec_addr);

	if (!vector_has_space(v_data))
	{
		v_data = vector_realloc(v_data, type_size);
		*vec_addr = v_data->buff;
	}

	return (void*)&v_data->buff[type_size * v_data->length++];
}

void* _vector_insert(vector* vec_addr, vec_type_t type_size, vec_size_t pos)
{
	vector_data* v_data = vector_get_data(*vec_addr);

	vec_size_t new_length = v_data->length + 1;

	// make sure there is enough room for the new element
	if (!vector_has_space(v_data))
	{
		v_data = vector_realloc(v_data, type_size);
	}
	memmove(&v_data->buff[(pos + 1) * type_size],
		&v_data->buff[pos * type_size],
		(v_data->length - pos) * type_size); // move trailing elements

	v_data->length = new_length;

	return &v_data->buff[pos * type_size];
}

void _vector_erase(vector* vec_addr, vec_type_t type_size, vec_size_t pos,
		   vec_size_t len)
{
	vector_data* v_data = vector_get_data(*vec_addr);
	// anyone who puts in a bad index can face the consequences on their own
	memmove(&v_data->buff[pos * type_size],
		&v_data->buff[(pos + len) * type_size],
		(v_data->length - pos - len) * type_size);

	v_data->length -= len;
}

void _vector_remove(vector* vec_addr, vec_type_t type_size, vec_size_t pos)
{
	_vector_erase(vec_addr, type_size, pos, 1);
}

void vector_pop(vector vec) { --vector_get_data(vec)->length; }

vector _vector_copy(vector vec, vec_type_t type_size)
{
	vector_data* vec_data = vector_get_data(vec);
	size_t alloc_size = sizeof(vector_data) + vec_data->length * type_size;
	vector_data* v = malloc(alloc_size);
	memcpy(v, vec_data, alloc_size);
	return (void*)&v->buff;
}