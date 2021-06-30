export declare enum WINDOW_FUNCTION {
	RECTANGULAR = 0,
	HANNING     = 1,
	HAMMING     = 2,
	BLACKMAN    = 3,
	TUKEY       = 4,
}


export declare const MFCC_DEFAULTS = {
	frameSize:  480,
	sampleRate: 16000,
	windowType: WINDOW_FUNCTION.HAMMING,
}


export declare interface MFCC_OPTIONS {
	frameSize: number;
	sampleRate: number;
	windowType: WINDOW_FUNCTION;
}


declare class MFCC {
	constructor(options: MFCC_OPTIONS);
	destroy(): void;
	getMFCC(frame: number[] | ArrayBufferView, filterBanks: number = 48, m: number = 13): Float64Array;
}


export default function(options: MFCC_OPTIONS): Promise<typeof MFCC>;


export type { MFCC };
