
/* vim: set et ts=3 sw=3 sts=3 ft=c:
 *
 * Copyright (C) 2014 James McLaughlin.  All rights reserved.
 * https://github.com/udp/json-builder
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include "stdafx.h"
#include "json-builder.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

#pragma warning(disable:4267)
#pragma warning(disable:4996)
#pragma warning(disable:4706)

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

    const static json_serialize_opts default_opts =
    {
       json_serialize_mode_single_line,
       0,
       3  /* indent_size */
    };

    typedef struct json_builder_value
    {
        json_value value;

        int is_builder_value;

        size_t additional_length_allocated;
        size_t length_iterated;

    } json_builder_value;

    void json_init()
    {
    }

    static int builderize(json_value * value)
    {
        if (((json_builder_value *)value)->is_builder_value)
            return 1;

        if (value->type == json_object)
        {
            unsigned int i;

            /* Values straight out of the parser have the names of object entries
             * allocated in the same allocation as the values array itself.  This is
             * not desirable when manipulating values because the names would be easy
             * to clobber.
             */
            for (i = 0; i < value->u.object.length; ++i)
            {
                json_char * name_copy;
                json_object_entry * entry = &value->u.object.values[i];

                if (!(name_copy = (json_char *)json_build_settings.mem_alloc((entry->name_length + 1) * sizeof(json_char), 0, 0)))
                    return 0;

                memcpy(name_copy, entry->name, entry->name_length + 1);
                entry->name = name_copy;
            }
        }

        ((json_builder_value *)value)->is_builder_value = 1;

        return 1;
    }

    const size_t json_builder_extra = sizeof(json_builder_value) - sizeof(json_value);

	static void * default_alloc(size_t size, int zero, void * user_data)
	{
		return zero ? calloc(1, size) : malloc(size);
	}

	static void * default_realloc(void * ptr, size_t size, int zero, void * user_data)
	{
		return realloc(ptr, size);
	}

	static void default_free(void * ptr, void * user_data)
	{
		free(ptr);
	}

	json_settings json_build_settings = {
		0,      /*max_memory*/
		0,      /*settings*/
		default_alloc, /*mem_alloc*/
		default_realloc, /*mem_realloc*/
		default_free, /*mem_free*/
		0, /*user_data*/
		0 /*value_extra*/
	};

    /* These flags are set up from the opts before serializing to make the
     * serializer conditions simpler.
     */
    const int f_spaces_around_brackets = (1 << 0);
    const int f_spaces_after_commas = (1 << 1);
    const int f_spaces_after_colons = (1 << 2);
    const int f_tabs = (1 << 3);

    int get_serialize_flags(json_serialize_opts opts)
    {
        int flags = 0;

        if (opts.mode == json_serialize_mode_packed)
            return 0;

        if (opts.mode == json_serialize_mode_multiline)
        {
            if (opts.opts & json_serialize_opt_use_tabs)
                flags |= f_tabs;
        }
        else
        {
            if (!(opts.opts & json_serialize_opt_pack_brackets))
                flags |= f_spaces_around_brackets;

            if (!(opts.opts & json_serialize_opt_no_space_after_comma))
                flags |= f_spaces_after_commas;
        }

        if (!(opts.opts & json_serialize_opt_no_space_after_colon))
            flags |= f_spaces_after_colons;

        return flags;
    }

    json_value * json_array_new(size_t length)
    {
        // DTEST: If we don't set the value to 0, json_array_push will crash.
        json_value * value = (json_value *)json_build_settings.mem_alloc(sizeof(json_builder_value), 1, 0);

        if (!value)
            return NULL;

        ((json_builder_value *)value)->is_builder_value = 1;

        value->type = json_array;

        if (!(value->u.array.values = (json_value **)json_build_settings.mem_alloc(length * sizeof(json_value *), 0, 0)))
        {
            json_build_settings.mem_free(value, 0);
            return NULL;
        }

        ((json_builder_value *)value)->additional_length_allocated = length;

        return value;
    }

    json_value * json_array_push(json_value * array, json_value * value)
    {
        assert(array->type == json_array);

        if (!builderize(array) || !builderize(value))
            return NULL;

        if (((json_builder_value *)array)->additional_length_allocated > 0)
        {
            --((json_builder_value *)array)->additional_length_allocated;
        }
        else
        {
            json_value ** values_new = (json_value **)json_build_settings.mem_realloc
            (array->u.array.values, sizeof(json_value *) * (array->u.array.length + 1), 0, 0);

            if (!values_new)
                return NULL;

            array->u.array.values = values_new;
        }

        array->u.array.values[array->u.array.length] = value;
        ++array->u.array.length;

        value->parent = array;

        return value;
    }

    json_value * json_object_new(size_t length)
    {
        size_t s = sizeof(json_builder_value);
        json_value * value = (json_value *)json_build_settings.mem_alloc(s, 0, 0);

        if (!value)
            return NULL;

        memset(value, 0, s);

        ((json_builder_value *)value)->is_builder_value = 1;

        value->type = json_object;

        s = length * sizeof(*value->u.object.values);
        if (!(value->u.object.values = (json_object_entry *)json_build_settings.mem_alloc(s, 0, 0)))
        {
            json_build_settings.mem_free(value, 0);
            return NULL;
        }

        memset(value->u.object.values, 0, s);

        ((json_builder_value *)value)->additional_length_allocated = length;

        return value;
    }

    json_value * json_object_push(json_value * object,
        const json_char * name,
        json_value * value)
    {
        return json_object_push_length(object, strlen(name), name, value);
    }

    json_value * json_object_push_length(json_value * object,
        unsigned int name_length, const json_char * name,
        json_value * value)
    {
        json_char * name_copy;

        assert(object->type == json_object);

        if (!(name_copy = (json_char *)json_build_settings.mem_alloc((name_length + 1) * sizeof(json_char), 0, 0)))
            return NULL;

        memcpy(name_copy, name, name_length * sizeof(json_char));
        name_copy[name_length] = 0;

        if (!json_object_push_nocopy(object, name_length, name_copy, value))
        {
            json_build_settings.mem_free(name_copy, 0);
            return NULL;
        }

        return value;
    }

    json_value * json_object_push_nocopy(json_value * object,
        unsigned int name_length, json_char * name,
        json_value * value)
    {
        json_object_entry * entry;

        assert(object->type == json_object);

        if (!builderize(object) || !builderize(value))
            return NULL;

        if (((json_builder_value *)object)->additional_length_allocated > 0)
        {
            --((json_builder_value *)object)->additional_length_allocated;
        }
        else
        {
            json_object_entry * values_new = (json_object_entry *)
                json_build_settings.mem_realloc(object->u.object.values, sizeof(*object->u.object.values)
                    * (object->u.object.length + 1), 0, 0);

            if (!values_new)
                return NULL;

            object->u.object.values = values_new;
        }

        entry = object->u.object.values + object->u.object.length;

        entry->name_length = name_length;
        entry->name = name;
        entry->value = value;

        ++object->u.object.length;

        value->parent = object;

        return value;
    }

    json_value * json_string_new(const json_char * buf)
    {
        return json_string_new_length(strlen(buf), buf);
    }

    json_value * json_string_new_length(unsigned int length, const json_char * buf)
    {
        json_value * value;
        json_char * copy = (json_char *)json_build_settings.mem_alloc((length + 1) * sizeof(json_char), 0, 0);

        if (!copy)
            return NULL;

        memcpy(copy, buf, length * sizeof(json_char));
        copy[length] = 0;

        if (!(value = json_string_new_nocopy(length, copy)))
        {
            json_build_settings.mem_free(copy, 0);
            return NULL;
        }

        return value;
    }

    json_value * json_string_new_nocopy(unsigned int length, json_char * buf)
    {
        size_t s = sizeof(json_builder_value);
        json_value * value = (json_value *)json_build_settings.mem_alloc(s, 0, 0);

        if (!value)
            return NULL;

        memset(value, 0, s);

        ((json_builder_value *)value)->is_builder_value = 1;

        value->type = json_string;
        value->u.string.length = length;
        value->u.string.ptr = buf;

        return value;
    }

    json_value * json_integer_new(json_int_t integer)
    {
        size_t s = sizeof(json_builder_value);
        json_value * value = (json_value *)json_build_settings.mem_alloc(s, 0, 0);

        if (!value)
            return NULL;

        memset(value, 0, s);

        ((json_builder_value *)value)->is_builder_value = 1;

        value->type = json_integer;
        value->u.integer = integer;

        return value;
    }

    json_value * json_unsigned_integer_new(json_uint_t unsigned_integer)
    {
        size_t s = sizeof(json_builder_value);
        json_value * value = (json_value *)json_build_settings.mem_alloc(s, 0, 0);

        if (!value)
            return NULL;

        memset(value, 0, s);

        ((json_builder_value *)value)->is_builder_value = 1;

        value->type = json_unsigned_integer;
        value->u.unsigned_integer = unsigned_integer;

        return value;
    }

    json_value * json_double_new(double dbl)
    {
        size_t s = sizeof(json_builder_value);
        json_value * value = (json_value *)json_build_settings.mem_alloc(s, 0, 0);

        if (!value)
            return NULL;

        memset(value, 0, s);

        ((json_builder_value *)value)->is_builder_value = 1;

        value->type = json_double;
        value->u.dbl = dbl;

        return value;
    }

    json_value * json_boolean_new(int b)
    {
        size_t s = sizeof(json_builder_value);
        json_value * value = (json_value *)json_build_settings.mem_alloc(s, 0, 0);

        if (!value)
            return NULL;

        memset(value, 0, s);

        ((json_builder_value *)value)->is_builder_value = 1;

        value->type = json_boolean;
        value->u.boolean = b;

        return value;
    }

    json_value * json_null_new()
    {
        size_t s = sizeof(json_builder_value);
        json_value * value = (json_value *)json_build_settings.mem_alloc(s, 0, 0);

        if (!value)
            return NULL;

        memset(value, 0, s);

        ((json_builder_value *)value)->is_builder_value = 1;

        value->type = json_null;

        return value;
    }

    void json_object_sort(json_value * object, json_value * proto)
    {
        unsigned int i, out_index = 0;

        if (!builderize(object))
            return;  /* TODO error */

        assert(object->type == json_object);
        assert(proto->type == json_object);

        for (i = 0; i < proto->u.object.length; ++i)
        {
            unsigned int j;
            json_object_entry proto_entry = proto->u.object.values[i];

            for (j = 0; j < object->u.object.length; ++j)
            {
                json_object_entry entry = object->u.object.values[j];

                if (entry.name_length != proto_entry.name_length)
                    continue;

                if (memcmp(entry.name, proto_entry.name, entry.name_length) != 0)
                    continue;

                object->u.object.values[j] = object->u.object.values[out_index];
                object->u.object.values[out_index] = entry;

                ++out_index;
            }
        }
    }

    json_value * json_object_merge(json_value * objectA, json_value * objectB)
    {
        unsigned int i;

        assert(objectA->type == json_object);
        assert(objectB->type == json_object);
        assert(objectA != objectB);

        if (!builderize(objectA) || !builderize(objectB))
            return NULL;

        if (objectB->u.object.length <=
            ((json_builder_value *)objectA)->additional_length_allocated)
        {
            ((json_builder_value *)objectA)->additional_length_allocated
                -= objectB->u.object.length;
        }
        else
        {
            json_object_entry * values_new;

            unsigned int alloc =
                objectA->u.object.length
                + ((json_builder_value *)objectA)->additional_length_allocated
                + objectB->u.object.length;

            if (!(values_new = (json_object_entry *)
                json_build_settings.mem_realloc(objectA->u.object.values, sizeof(json_object_entry) * alloc, 0, 0)))
            {
                return NULL;
            }

            objectA->u.object.values = values_new;
        }

        for (i = 0; i < objectB->u.object.length; ++i)
        {
            json_object_entry * entry = &objectA->u.object.values[objectA->u.object.length + i];

            *entry = objectB->u.object.values[i];
            entry->value->parent = objectA;
        }

        objectA->u.object.length += objectB->u.object.length;

        json_build_settings.mem_free(objectB->u.object.values, 0);
        json_build_settings.mem_free(objectB, 0);

        return objectA;
    }

    static size_t measure_string(unsigned int length,
        const json_char * str)
    {
        unsigned int i;
        size_t measured_length = 0;

        for (i = 0; i < length; ++i)
        {
            json_char c = str[i];

            switch (c)
            {
            case '"':
            case '\\':
            case '\b':
            case '\f':
            case '\n':
            case '\r':
            case '\t':

                measured_length += 2;
                break;

            default:

                ++measured_length;
                break;
            };
        };

        return measured_length;
    }

#define PRINT_ESCAPED(c) do {  \
   *buf ++ = '\\';             \
   *buf ++ = (c);              \
} while(0);                    \

    static size_t serialize_string(json_char * buf,
        unsigned int length,
        const json_char * str)
    {
        json_char * orig_buf = buf;
        unsigned int i;

        for (i = 0; i < length; ++i)
        {
            json_char c = str[i];

            switch (c)
            {
            case '"':   PRINT_ESCAPED('\"');  continue;
            case '\\':  PRINT_ESCAPED('\\');  continue;
            case '\b':  PRINT_ESCAPED('b');   continue;
            case '\f':  PRINT_ESCAPED('f');   continue;
            case '\n':  PRINT_ESCAPED('n');   continue;
            case '\r':  PRINT_ESCAPED('r');   continue;
            case '\t':  PRINT_ESCAPED('t');   continue;

            default:

                *buf++ = c;
                break;
            };
        };

        return buf - orig_buf;
    }

    size_t json_measure(json_value * value)
    {
        return json_measure_ex(value, default_opts);
    }

#define MEASURE_NEWLINE() do {                     \
   ++ newlines;                                    \
   indents += depth;                               \
} while(0);                                        \

    size_t json_measure_ex(json_value * value, json_serialize_opts opts)
    {
        json_value* start_value = value;
        json_value* start_parent = nullptr;
        if (start_value)
        {
            start_parent = start_value->parent;
            start_value->parent = nullptr;
        }

        size_t total = 1;  /* null terminator */
        size_t newlines = 0;
        size_t depth = 0;
        size_t indents = 0;
        int flags;
        int bracket_size, comma_size, colon_size;

        flags = get_serialize_flags(opts);

        /* to reduce branching */
        bracket_size = flags & f_spaces_around_brackets ? 2 : 1;
        comma_size = flags & f_spaces_after_commas ? 2 : 1;
        colon_size = flags & f_spaces_after_colons ? 2 : 1;

        while (value)
        {
            json_uint_t unsigned_integer;
            json_object_entry * entry;

            switch (value->type)
            {
            case json_array:

                if (((json_builder_value *)value)->length_iterated == 0)
                {
                    if (value->u.array.length == 0)
                    {
                        total += 2;  /* `[]` */
                        break;
                    }

                    total += bracket_size;  /* `[` */

                    ++depth;
                    MEASURE_NEWLINE(); /* \n after [ */
                }

                if (((json_builder_value *)value)->length_iterated == value->u.array.length)
                {
                    --depth;
                    MEASURE_NEWLINE();
                    total += bracket_size;  /* `]` */

                    ((json_builder_value *)value)->length_iterated = 0;
                    break;
                }

                if (((json_builder_value *)value)->length_iterated > 0)
                {
                    total += comma_size;  /* `, ` */

                    MEASURE_NEWLINE();
                }

                ((json_builder_value *)value)->length_iterated++;
                value = value->u.array.values[((json_builder_value *)value)->length_iterated - 1];
                continue;

            case json_object:

                if (((json_builder_value *)value)->length_iterated == 0)
                {
                    if (value->u.object.length == 0)
                    {
                        total += 2;  /* `{}` */
                        break;
                    }

                    total += bracket_size;  /* `{` */

                    ++depth;
                    MEASURE_NEWLINE(); /* \n after { */
                }

                if (((json_builder_value *)value)->length_iterated == value->u.object.length)
                {
                    --depth;
                    MEASURE_NEWLINE();
                    total += bracket_size;  /* `}` */

                    ((json_builder_value *)value)->length_iterated = 0;
                    break;
                }

                if (((json_builder_value *)value)->length_iterated > 0)
                {
                    total += comma_size;  /* `, ` */
                    MEASURE_NEWLINE();
                }

                entry = value->u.object.values + (((json_builder_value *)value)->length_iterated++);

                total += 2 + colon_size;  /* `"": ` */
                total += measure_string(entry->name_length, entry->name);

                value = entry->value;
                continue;

            case json_string:

                total += 2;  /* `""` */
                total += measure_string(value->u.string.length, value->u.string.ptr);
                break;

            case json_integer:
            case json_unsigned_integer:

                if (value->type == json_integer)
                {
                    if (value->u.integer < 0)
                    {
                        total += 1;  /* `-` */
                        unsigned_integer = -value->u.integer;
                    }
                    else
                    {
                        unsigned_integer = value->u.integer;
                    }
                }
                else
                {
                    unsigned_integer = value->u.unsigned_integer;
                }

                ++total;  /* first digit */

                while (unsigned_integer >= 10)
                {
                    ++total;  /* another digit */
                    unsigned_integer /= 10;
                }

                break;

            case json_double:
                // DTEST: Changed to use %lf instead of %g, which was 
                // losing precision for some doubles.
                total += snprintf(NULL, 0, "%lf", value->u.dbl);

                // DTEST: No more need for this check as %lf always adds 
                // something after the decimals dot.
    //             if (value->u.dbl - floor (value->u.dbl) < 0.001)
    //                 total += 2;

                break;

            case json_boolean:

                total += value->u.boolean ?
                    4 :  /* `true` */
                    5;  /* `false` */

                break;

            case json_null:

                total += 4;  /* `null` */
                break;

            default:
                break;
            };

            value = value->parent;
        }

        if (opts.mode == json_serialize_mode_multiline)
        {
            total += newlines * (((opts.opts & json_serialize_opt_CRLF) ? 2 : 1) + opts.indent_size);
            total += indents * opts.indent_size;
        }

        if (start_value)
        {
            start_value->parent = start_parent;
        }

        return total;
    }

    void json_serialize(json_char * buf, json_value * value)
    {
        json_serialize_ex(buf, value, default_opts);
    }

#define PRINT_NEWLINE() do {                          \
   if (opts.mode == json_serialize_mode_multiline) {  \
      if (opts.opts & json_serialize_opt_CRLF)        \
         *buf ++ = '\r';                              \
      *buf ++ = '\n';                                 \
      for(i = 0; i < indent; ++ i)                    \
         *buf ++ = indent_char;                       \
   }                                                  \
} while(0);                                           \

#define PRINT_OPENING_BRACKET(c) do {                 \
   *buf ++ = (c);                                     \
   if (flags & f_spaces_around_brackets)              \
      *buf ++ = ' ';                                  \
} while(0);                                           \

#define PRINT_CLOSING_BRACKET(c) do {                 \
   if (flags & f_spaces_around_brackets)              \
      *buf ++ = ' ';                                  \
   *buf ++ = (c);                                     \
} while(0);                                           \

    void json_serialize_ex(json_char * buf, json_value * value, json_serialize_opts opts)
    {
        json_value* start_value = value;
        json_value* start_parent = nullptr;
        if (start_value)
        {
            start_parent = start_value->parent;
            start_value->parent = nullptr;
        }

        json_uint_t unsigned_integer, orig_unsigned_integer;
        json_object_entry * entry;
        json_char * ptr, *dot;
        int indent = 0;
        char indent_char;
        int i;
        int flags;

        flags = get_serialize_flags(opts);

        indent_char = flags & f_tabs ? '\t' : ' ';

        while (value)
        {
            switch (value->type)
            {
            case json_array:

                if (((json_builder_value *)value)->length_iterated == 0)
                {
                    if (value->u.array.length == 0)
                    {
                        *buf++ = '[';
                        *buf++ = ']';

                        break;
                    }

                    PRINT_OPENING_BRACKET('[');

                    indent += opts.indent_size;
                    PRINT_NEWLINE();
                }

                if (((json_builder_value *)value)->length_iterated == value->u.array.length)
                {
                    indent -= opts.indent_size;
                    PRINT_NEWLINE();
                    PRINT_CLOSING_BRACKET(']');

                    ((json_builder_value *)value)->length_iterated = 0;
                    break;
                }

                if (((json_builder_value *)value)->length_iterated > 0)
                {
                    *buf++ = ',';

                    if (flags & f_spaces_after_commas)
                        *buf++ = ' ';

                    PRINT_NEWLINE();
                }

                ((json_builder_value *)value)->length_iterated++;
                value = value->u.array.values[((json_builder_value *)value)->length_iterated - 1];
                continue;

            case json_object:

                if (((json_builder_value *)value)->length_iterated == 0)
                {
                    if (value->u.object.length == 0)
                    {
                        *buf++ = '{';
                        *buf++ = '}';

                        break;
                    }

                    PRINT_OPENING_BRACKET('{');

                    indent += opts.indent_size;
                    PRINT_NEWLINE();
                }

                if (((json_builder_value *)value)->length_iterated == value->u.object.length)
                {
                    indent -= opts.indent_size;
                    PRINT_NEWLINE();
                    PRINT_CLOSING_BRACKET('}');

                    ((json_builder_value *)value)->length_iterated = 0;
                    break;
                }

                if (((json_builder_value *)value)->length_iterated > 0)
                {
                    *buf++ = ',';

                    if (flags & f_spaces_after_commas)
                        *buf++ = ' ';

                    PRINT_NEWLINE();
                }

                entry = value->u.object.values + (((json_builder_value *)value)->length_iterated++);

                *buf++ = '\"';
                buf += serialize_string(buf, entry->name_length, entry->name);
                *buf++ = '\"';
                *buf++ = ':';

                if (flags & f_spaces_after_colons)
                    *buf++ = ' ';

                value = entry->value;
                continue;

            case json_string:

                *buf++ = '\"';
                buf += serialize_string(buf, value->u.string.length, value->u.string.ptr);
                *buf++ = '\"';
                break;

            case json_integer:
            case json_unsigned_integer:

                if (value->type == json_integer)
                {
                    if (value->u.integer < 0)
                    {
                        *buf++ = '-';
                        unsigned_integer = -value->u.integer;
                    }
                    else
                    {
                        unsigned_integer = value->u.integer;
                    }
                }
                else
                {
                    unsigned_integer = value->u.unsigned_integer;
                }

                orig_unsigned_integer = unsigned_integer;

                ++buf;

                while (unsigned_integer >= 10)
                {
                    ++buf;
                    unsigned_integer /= 10;
                }

                unsigned_integer = orig_unsigned_integer;
                ptr = buf;

                do
                {
                    *--ptr = "0123456789"[unsigned_integer % 10];

                } while ((unsigned_integer /= 10) > 0);

                break;

            case json_double:

                ptr = buf;

                // DTEST: Changed to use %lf instead of %g, which was 
                // losing precision for some doubles.
                buf += sprintf(buf, "%lf", value->u.dbl);

                if ((dot = strchr(ptr, ',')))
                {
                    *dot = '.';
                }
                // DTEST: No more need for this check as %lf always adds 
                // something after the decimals dot.
    //             else if (!strchr (ptr, '.'))
    //             {
    //                *buf ++ = '.';
    //                *buf ++ = '0';
    //             }

                break;

            case json_boolean:

                if (value->u.boolean)
                {
                    memcpy(buf, "true", 4);
                    buf += 4;
                }
                else
                {
                    memcpy(buf, "false", 5);
                    buf += 5;
                }

                break;

            case json_null:

                memcpy(buf, "null", 4);
                buf += 4;
                break;

            default:
                break;
            };

            value = value->parent;
        }

        *buf = 0;
        if (start_value)
        {
            start_value->parent = start_parent;
        }
    }

    void json_builder_free(json_value * value)
    {
        json_value * cur_value;

        if (!value)
            return;

        value->parent = 0;

        while (value)
        {
            switch (value->type)
            {
            case json_array:

                if (!value->u.array.length)
                {
                    // DTEST: Added check for when deleting an empty array to avoid crash when calling mem_free on NULL.
                    // This fix is already done in json_value_free_ex but for some reason here it wasn't.
                    if (value->u.array.values)
                    {
                        json_build_settings.mem_free(value->u.array.values, 0);
                    }
                    break;
                }

                value = value->u.array.values[--value->u.array.length];
                continue;

            case json_object:

                if (!value->u.object.length)
                {
                    // DTEST: Added check for when deleting an empty object to avoid crash when calling mem_free on NULL.
                    // This fix is already done in json_value_free_ex but for some reason here it wasn't.
                    if (value->u.object.values)
                    {
                        json_build_settings.mem_free(value->u.object.values, 0);
                    }
                    break;
                }

                --value->u.object.length;

                if (((json_builder_value *)value)->is_builder_value)
                {
                    /* Names are allocated separately for builder values.  In parser
                     * values, they are part of the same allocation as the values array
                     * itself.
                     */
                    json_build_settings.mem_free(value->u.object.values[value->u.object.length].name, 0);
                }

                value = value->u.object.values[value->u.object.length].value;
                continue;

            case json_string:
                // DTEST: Added check for when deleting an empty string to avoid crash when calling mem_free on NULL.
                // This fix is already done in json_value_free_ex but for some reason here it wasn't.
                if (value->u.string.ptr)
                {
                    json_build_settings.mem_free(value->u.string.ptr, 0);
                }
                break;

            default:
                break;
            };

            cur_value = value;
            value = value->parent;
            json_build_settings.mem_free(cur_value, 0);
        }
    }
