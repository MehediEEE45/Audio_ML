/* mfcc.h
 * Simple MFCC extractor for short PCM frames (16-bit)
 * This implementation uses a straightforward DFT (no external DSP libs)
 * and a Mel filterbank + DCT-II to produce MFCCs for a single frame.
 *
 * Usage:
 *  - include "mfcc.h"
 *  - call `mfcc_compute()` with a pointer to int16 PCM samples (frame) and
 *    an output buffer of size `num_coeffs` floats.
 *
 * Notes:
 *  - This is intended for moderate frame sizes (e.g. 256..1024). The DFT is
 *    O(N^2) and may be slow for very large frames on MCU. For better
 *    performance use CMSIS-DSP FFT primitives.
 */

#ifndef INC_MFCC_H_
#define INC_MFCC_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Default parameters */
#define MFCC_DEFAULT_NMEL     26
#define MFCC_DEFAULT_NCOEFF   13

/* Compute MFCCs for a single PCM frame.
 * pcm_frame: pointer to int16_t PCM samples (length = frame_len)
 * frame_len: number of samples in the frame
 * sample_rate: sampling rate in Hz (e.g. 16000)
 * num_mel: number of mel filters (e.g. 26)
 * num_coeff: number of MFCC coefficients to produce (e.g. 13)
 * mfcc_out: pointer to float buffer of length num_coeff (output)
 * returns 0 on success, non-zero on error
 */
int mfcc_compute(const int16_t *pcm_frame, uint32_t frame_len, uint32_t sample_rate,
                 uint32_t num_mel, uint32_t num_coeff, float *mfcc_out);

/* Compute log-mel filterbank energies for a single PCM frame.
 * This returns `num_mel` log-energy values (natural log).
 * pcm_frame: pointer to int16_t PCM samples (length = frame_len)
 * frame_len: number of samples in the frame
 * sample_rate: sampling rate in Hz (e.g. 16000)
 * num_mel: number of mel filters / output features (e.g. 40)
 * logmel_out: pointer to float buffer of length num_mel (output)
 * returns 0 on success, non-zero on error
 */
int mfcc_compute_logmel(const int16_t *pcm_frame, uint32_t frame_len, uint32_t sample_rate,
                        uint32_t num_mel, float *logmel_out);

#ifdef __cplusplus
}
#endif

#endif /* INC_MFCC_H_ */
