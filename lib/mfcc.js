export const WINDOW_FUNCTION = Object.freeze({
	RECTANGULAR: 0,
	HANNING:     1,
	HAMMING:     2,
	BLACKMAN:    3,
	TUKEY:       4,
});


export const MFCC_DEFAULTS = Object.freeze({
	frameSize: 480,
	sampleRate: 16000,
	windowType: WINDOW_FUNCTION.HAMMING,
});


export async function mfccInit(binary, options) {
	const { instance } = await WebAssembly.instantiate(binary);
	const mfcc = instance.exports;


	class MFCC {
		constructor(options) {
			this.destroyed  = false;
			this.frameSize  = options.frameSize || MFCC_DEFAULTS.frameSize;
			this.sampleRate = options.sampleRate || MFCC_DEFAULTS.sampleRate;
			this.windowType = options.windowType || MFCC_DEFAULTS.windowType;
			mfcc.init_mfcc(this.frameSize, this.sampleRate, this.windowType);
		}

		destroy() {
			mfcc.free_mfcc();
			this.destroyed = true;
		}

		getMFCC(frame, filterBanks = 48, m = 13) {
			if (this.destroyed) {
				throw new Error("Object already destroyed");
			}
			const doubles = Float64Array.from(frame);
			const bytes   = doubles.length * doubles.BYTES_PER_ELEMENT;
			const ptr     = mfcc.malloc(bytes);
			const data    = new Float64Array(mfcc.memory.buffer, ptr, bytes);
			data.set(doubles);

			const out = mfcc.get_mfcc(data.byteOffset, filterBanks, m);
			const ret = new Float64Array(m);
			ret.set(new Float64Array(mfcc.memory.buffer, out, m));
			mfcc.free(out);
			mfcc.free(data.byteOffset);

			return ret;
		}
	}

	return new MFCC(options);
}
