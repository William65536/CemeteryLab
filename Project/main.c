#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <math.h>

/* Maybe use macros instead */
// static const size_t num_elms = 612; /* ? */

typedef struct StringView {
    const char *str_ref;
    size_t size;
} StringView;

/* A month is a string with an implicit length of 3 */
typedef const char *Month;

typedef struct Element2 {
    struct {
        StringView first;
        StringView last;
    } name;
    struct {
        Month month;
        unsigned int day;
        unsigned int year;
    } date;
    double age;
    struct {
        StringView area;
        StringView street;
        double number;
    } address;
} Element2;

typedef struct Element {
    // const char *line_offset;
    StringView name;
    struct {
        Month month;
        unsigned int day;
        signed   int year;
    } date;
    double age;
    struct {
        StringView street;
        double number;
    } address;
} Element;

/* For debugging purposes */
void StringView_println(StringView sv)
{
    assert(sv.str_ref != NULL);

    for (size_t i = 0; i < sv.size; i++)
        putchar(sv.str_ref[i]);

    putchar('\n');
}

static inline bool is_alpha(char c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

static inline bool is_digit(char c)
{
    return '0' <= c && c <= '9';
}

/* Assumes correctness of input */
static Element readline(const char **itr)
{
    Element ret;

    const char *start = *itr;

    {
        static const size_t name_offset = 0;

        *itr = start + name_offset;

        ret.name.str_ref = *itr;

        for (ret.name.size = 0; is_alpha(**itr) || (**itr == ' ' && is_alpha(*(*itr + 1))); ++*itr, ret.name.size++);
    }

    {
        static const size_t date_offset = 25;

        *itr = start + date_offset;

        ret.date.day = (**itr - '0') * 10 + *(*itr + 1) - '0';

        *itr += 2 + 1;
        ret.date.month = *itr;

        *itr += 3 + 1;
        ret.date.year = (**itr - '0') * 1000 + (*(*itr + 1) - '0') * 100 + (*(*itr + 2) - '0') * 10 + *(*itr + 3) - '0';
    }

    {
        static const size_t age_offset = 37;

        *itr = start + age_offset;

        unsigned int integral = 0;
        unsigned int frac = 0, divisor = 1;

        for (; is_digit(**itr); ++*itr)
            integral = integral * 10 + **itr - '0';

        if (**itr != '.')
            goto assign_age;

        ++*itr;

        for (; is_digit(**itr); ++*itr, divisor *= 10)
            frac = frac * 10 + **itr - '0';

        assign_age:

        ret.age = integral + (double) frac / divisor;
    }

    {
        static const size_t address_offset = 42;

        *itr = start + address_offset;

        ret.address.number = 0.0;

        for (; is_digit(**itr); ++*itr)
            ret.address.number = ret.address.number * 10.0 + **itr - '0';

        if (*(*itr + 1) == '1') {
            ret.address.number += 0.5;
            *itr += 1 + 3;
        }

        while (**itr == ' ' || **itr == ',')
            ++*itr;

        ret.address.street.str_ref = *itr;

        /* This doesn't even attempt to handle trailing whitespace, but this isn't a real problem */
        for (ret.address.street.size = 0; **itr != '\n'; ++*itr, ret.address.street.size++);
    }

    return ret;
}

int main(void)
{
    struct {
        size_t size;
        char *buf;
    } data;

    {
        FILE *file = fopen("cemetery_orig.txt", "rb");

        if (file == NULL) {
            fputs("FATAL ERROR: Unable to open file\n", stderr);
            exit(EXIT_FAILURE);
        }

        if (fseek(file, 0, SEEK_END)) {
            fclose(file);
            fputs("FATAL ERROR: File cannot be used as a stream\n", stderr);
            exit(EXIT_FAILURE);
        }

        data.size = ftell(file);

        rewind(file);

        data.buf = malloc(data.size * sizeof *data.buf);

        if (data.buf == NULL) {
            fclose(file);
            fputs("FATAL ERROR: Failed memory allocation\n", stderr);
            exit(EXIT_FAILURE);
        }

        if (fread(data.buf, data.size, 1, file) != 1) {
            fclose(file);
            free(data.buf);
            fputs("FATAL ERROR: Failed read from file\n", stderr);
            exit(EXIT_FAILURE);
        }

        fclose(file);
    }

    {
        static const size_t lines_skip = 4;

        const char *itr = data.buf;

        for (size_t i = 0; i < lines_skip; itr++)
            if (*itr == '\n')
                i++;

        Element el = readline(&itr);

        // printf("%d\n", );

        StringView_println(el.address.street);

    }





    free(data.buf);

    return EXIT_SUCCESS;
}
