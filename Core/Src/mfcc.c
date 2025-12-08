/* mfcc.c
 * Simple MFCC implementation for single-frame extraction.
 * See header for usage notes.
 */

#include "mfcc.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846f
#define EPS 1e-10f

/* Convert frequency (Hz) to Mel scale */
static float hz_to_mel(float hz)
{
    return 2595.0f * log10f(1.0f + hz / 700.0f);
}

/* Convert Mel to frequency (Hz) */
static float mel_to_hz(float mel)
{
    return 700.0f * (powf(10.0f, mel / 2595.0f) - 1.0f);
}

/* Next power of two >= v */
static uint32_t next_pow2(uint32_t v)
{
    uint32_t p = 1;
    while (p < v) p <<= 1;
    return p;
}

int mfcc_compute(const int16_t *pcm_frame, uint32_t frame_len, uint32_t sample_rate,
                 uint32_t num_mel, uint32_t num_coeff, float *mfcc_out)
{
    if (!pcm_frame || !mfcc_out || frame_len == 0 || num_mel == 0 || num_coeff == 0) return -1;

    /* FFT length (zero-pad if necessary) */
    uint32_t N = next_pow2(frame_len);
    uint32_t fft_bins = (N / 2) + 1;

    /* allocate working buffers */
    float *x = (float*)malloc(sizeof(float) * N); /* windowed signal, zero-padded */
    float *power = (float*)malloc(sizeof(float) * fft_bins);
    if (!x || !power) { free(x); free(power); return -2; }

    /* Hamming window + convert to float */
    for (uint32_t n = 0; n < frame_len; n++)
    {
        float w = 0.54f - 0.46f * cosf((2.0f * PI * n) / (frame_len - 1));
        x[n] = ((float)pcm_frame[n]) * w;
    }
    for (uint32_t n = frame_len; n < N; n++) x[n] = 0.0f;

    /* Compute DFT (naive O(N^2)) for bins 0..N/2 */
    for (uint32_t k = 0; k < fft_bins; k++)
    {
        float re = 0.0f, im = 0.0f;
        for (uint32_t n = 0; n < N; n++)
        {
            float angle = 2.0f * PI * ((float)k * (float)n) / (float)N;
            re += x[n] * cosf(angle);
            im -= x[n] * sinf(angle);
        }
        power[k] = re * re + im * im;
    }

    /* Build Mel filterbank */
    float f_min = 0.0f;
    float f_max = (float)sample_rate / 2.0f;
    float mel_min = hz_to_mel(f_min);
    float mel_max = hz_to_mel(f_max);

    float *mel_points = (float*)malloc(sizeof(float) * (num_mel + 2));
    uint32_t *bin = (uint32_t*)malloc(sizeof(uint32_t) * (num_mel + 2));
    if (!mel_points || !bin) { free(x); free(power); free(mel_points); free(bin); return -3; }

    for (uint32_t i = 0; i < num_mel + 2; i++)
    {
        float m = mel_min + ((float)i) * (mel_max - mel_min) / (float)(num_mel + 1);
        mel_points[i] = mel_to_hz(m);
        /* map to nearest FFT bin */
        uint32_t b = (uint32_t)floorf((mel_points[i] / f_max) * (float)(fft_bins - 1) + 0.5f);
        if (b >= fft_bins) b = fft_bins - 1;
        bin[i] = b;
    }

    /* allocate filter weights flattened: num_mel * fft_bins */
    float *filters = (float*)malloc(sizeof(float) * num_mel * fft_bins);
    if (!filters) { free(x); free(power); free(mel_points); free(bin); return -4; }
    memset(filters, 0, sizeof(float) * num_mel * fft_bins);

    for (uint32_t m = 1; m <= num_mel; m++)
    {
        uint32_t f_m_minus = bin[m - 1];
        uint32_t f_m = bin[m];
        uint32_t f_m_plus = bin[m + 1];

        if (f_m == f_m_minus) f_m = f_m_minus + 1;
        for (uint32_t k = f_m_minus; k <= f_m; k++)
        {
            float val = 0.0f;
            if (f_m != f_m_minus)
                val = ((float)(k - f_m_minus)) / ((float)(f_m - f_m_minus));
            filters[(m - 1) * fft_bins + k] = val;
        }
        for (uint32_t k = f_m; k <= f_m_plus; k++)
        {
            float val = 0.0f;
            if (f_m_plus != f_m)
                val = ((float)(f_m_plus - k)) / ((float)(f_m_plus - f_m));
            filters[(m - 1) * fft_bins + k] = val;
        }
    }

    /* Apply filters to power spectrum -> mel energies */
    float *mel_energies = (float*)malloc(sizeof(float) * num_mel);
    if (!mel_energies) { free(x); free(power); free(mel_points); free(bin); free(filters); return -5; }
    for (uint32_t m = 0; m < num_mel; m++)
    {
        float sum = 0.0f;
        for (uint32_t k = 0; k < fft_bins; k++) sum += power[k] * filters[m * fft_bins + k];
        if (sum < EPS) sum = EPS;
        mel_energies[m] = logf(sum);
    }

    /* DCT-II to produce MFCCs */
    for (uint32_t i = 0; i < num_coeff; i++)
    {
        float s = 0.0f;
        for (uint32_t j = 0; j < num_mel; j++)
        {
            s += mel_energies[j] * cosf(PI * (float)i * ((float)j + 0.5f) / (float)num_mel);
        }
        mfcc_out[i] = s;
    }

    /* cleanup */
    free(x);
    free(power);
    free(mel_points);
    free(bin);
    free(filters);
    free(mel_energies);

    return 0;
}


int mfcc_compute_logmel(const int16_t *pcm_frame, uint32_t frame_len, uint32_t sample_rate,
                        uint32_t num_mel, float *logmel_out)
{
    if (!pcm_frame || !logmel_out || frame_len == 0 || num_mel == 0) return -1;

    uint32_t N = next_pow2(frame_len);
    uint32_t fft_bins = (N / 2) + 1;

    /* allocate working buffers */
    float *x = (float*)malloc(sizeof(float) * N);
    float *power = (float*)malloc(sizeof(float) * fft_bins);
    if (!x || !power) { free(x); free(power); return -2; }

    /* Hamming window + convert to float */
    for (uint32_t n = 0; n < frame_len; n++)
    {
        float w = 0.54f - 0.46f * cosf((2.0f * PI * n) / (frame_len - 1));
        x[n] = ((float)pcm_frame[n]) * w;
    }
    for (uint32_t n = frame_len; n < N; n++) x[n] = 0.0f;

    /* Compute DFT (naive) and power spectrum */
    for (uint32_t k = 0; k < fft_bins; k++)
    {
        float re = 0.0f, im = 0.0f;
        for (uint32_t n = 0; n < N; n++)
        {
            float angle = 2.0f * PI * ((float)k * (float)n) / (float)N;
            re += x[n] * cosf(angle);
            im -= x[n] * sinf(angle);
        }
        power[k] = re * re + im * im;
    }

    /* Build mel bin centers */
    float f_min = 0.0f;
    float f_max = (float)sample_rate / 2.0f;
    float mel_min = hz_to_mel(f_min);
    float mel_max = hz_to_mel(f_max);

    float *mel_hz = (float*)malloc(sizeof(float) * (num_mel + 2));
    uint32_t *bin = (uint32_t*)malloc(sizeof(uint32_t) * (num_mel + 2));
    if (!mel_hz || !bin) { free(x); free(power); free(mel_hz); free(bin); return -3; }

    for (uint32_t i = 0; i < num_mel + 2; i++)
    {
        float m = mel_min + ((float)i) * (mel_max - mel_min) / (float)(num_mel + 1);
        mel_hz[i] = mel_to_hz(m);
        uint32_t b = (uint32_t)floorf((mel_hz[i] / f_max) * (float)(fft_bins - 1) + 0.5f);
        if (b >= fft_bins) b = fft_bins - 1;
        bin[i] = b;
    }

    /* For each mel filter, compute triangular-weighted energy from power spectrum */
    for (uint32_t m = 1; m <= num_mel; m++)
    {
        uint32_t start = bin[m - 1];
        uint32_t center = bin[m];
        uint32_t end = bin[m + 1];
        float sum = 0.0f;

        /* ascending slope */
        if (center > start)
        {
            for (uint32_t k = start; k <= center; k++)
            {
                float w = ((float)(k - start)) / ((float)(center - start));
                sum += power[k] * w;
            }
        }
        else if (center == start)
        {
            /* single bin center */
            sum += power[center];
        }

        /* descending slope */
        if (end > center)
        {
            for (uint32_t k = center; k <= end; k++)
            {
                float w = ((float)(end - k)) / ((float)(end - center));
                sum += power[k] * w;
            }
        }

        if (sum < EPS) sum = EPS;
        logmel_out[m - 1] = logf(sum);
    }

    free(x);
    free(power);
    free(mel_hz);
    free(bin);

    return 0;
}
