/*
 * k0rt.c — k0 runtime support library
 *
 * Provides C implementations of the predefined k0 functions so that
 * generated assembly can be linked into a working executable.
 *
 * Compile and link alongside generated assembly:
 *   gcc <program>.s k0rt.c -o <program>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Global-initialiser fallback ──────────────────────────────────────────── *
 * The k0 compiler emits an __init function that runs global initialisers.
 * When no globals with initializers exist, this weak symbol is used instead. */
void __init(void) __attribute__((weak));
void __init(void) { /* nothing to do */ }

/* ── I/O ──────────────────────────────────────────────────────────────────── */

/* println(s: String) — print with trailing newline */
void println(char *s)
{
    puts(s ? s : "");
}

/* print(s: String) — print without newline */
void print(char *s)
{
    fputs(s ? s : "", stdout);
    fflush(stdout);
}

/* readln(): String — read one line of input (newline stripped) */
char *readln(void)
{
    char buf[4096];
    char *p = fgets(buf, sizeof(buf), stdin);
    if (!p)
        return strdup("");
    size_t n = strlen(buf);
    if (n > 0 && buf[n - 1] == '\n')
        buf[n - 1] = '\0';
    return strdup(buf);
}

/* ── String operations ────────────────────────────────────────────────────── */

/* length(s): Int */
long long length(char *s)
{
    return s ? (long long)strlen(s) : 0LL;
}

/* get(s, i): Char — character at index i */
long long get(char *s, long long i)
{
    if (!s || i < 0 || i >= (long long)strlen(s))
        return 0;
    return (long long)(unsigned char)s[i];
}

/* substring(s, start, end): String */
char *substring(char *s, long long start, long long end)
{
    if (!s)
        return strdup("");
    long long len = (long long)strlen(s);
    if (start < 0)
        start = 0;
    if (end > len)
        end = len;
    if (start >= end)
        return strdup("");
    long long newlen = end - start;
    char *result = malloc((size_t)(newlen + 1));
    memcpy(result, s + start, (size_t)newlen);
    result[newlen] = '\0';
    return result;
}

/* equals(a, b): Boolean */
long long equals(char *a, char *b)
{
    if (a == b)
        return 1;
    if (!a || !b)
        return 0;
    return (long long)(strcmp(a, b) == 0);
}

/* toString(n): String — integer to decimal string */
char *toString(long long n)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "%lld", n);
    return strdup(buf);
}

/* valueOf(s): String — identity for strings (or null-safe copy) */
char *valueOf(char *s)
{
    return s ? strdup(s) : strdup("null");
}

/* ── Math ─────────────────────────────────────────────────────────────────── */

/* abs(x): Int */
long long abs_k0(long long x)
{
    return x < 0 ? -x : x;
}

/* max(a, b): Int */
long long max(long long a, long long b)
{
    return a > b ? a : b;
}

/* min(a, b): Int */
long long min(long long a, long long b)
{
    return a < b ? a : b;
}

/*
 * pow, cos, sin, tan — these are provided as simple stubs so that k0rt.c
 * links without -lm for programs that don't use them.
 *
 * Programs that need accurate math should link with -lm and the real libm
 * symbols will override these weak definitions at link time.
 */
double pow_k0(double base, double exp) __attribute__((weak));
double pow_k0(double base, double exp)
{
    /* integer-exponent fast path; real programs should link -lm */
    double result = 1.0;
    int n = (int)exp;
    for (int i = 0; i < n; i++)
        result *= base;
    return result;
}

double cos_k0(double x) __attribute__((weak));
double cos_k0(double x)
{
    /* 4-term Taylor around 0 — enough for a stub */
    double x2 = x * x;
    return 1.0 - x2 / 2.0 + x2 * x2 / 24.0 - x2 * x2 * x2 / 720.0;
}

double sin_k0(double x) __attribute__((weak));
double sin_k0(double x)
{
    double x2 = x * x;
    return x - x * x2 / 6.0 + x * x2 * x2 / 120.0 - x * x2 * x2 * x2 / 5040.0;
}

double tan_k0(double x) __attribute__((weak));
double tan_k0(double x)
{
    double c = cos_k0(x);
    return (c != 0.0) ? sin_k0(x) / c : 0.0;
}

/* nextInt(n): Int — returns a pseudo-random integer in [0, n) */
long long nextInt(long long n)
{
    if (n <= 0)
        return 0;
    return (long long)(rand() % (int)n);
}

/* ── Arrays (arrayOf stub) ────────────────────────────────────────────────── *
 * Full generic arrayOf requires runtime type info; this stub returns NULL.   *
 * Generated code that uses arrays will call this — extend as needed.         */
void *arrayOf(void)
{
    return NULL;
}