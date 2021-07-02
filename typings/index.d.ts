export declare const MFCC_DEFAULTS: MFCC_OPTIONS;


export declare interface MFCC_OPTIONS {
	frameSize?: number;
	sampleRate?: number;
}


declare class MFCC {
	constructor(options: MFCC_OPTIONS);
	destroy(): void;
	getMFCC(frame: number[] | ArrayBufferView, filterBanks?: number, m?: number): Float64Array;
}


export default function(options: MFCC_OPTIONS): Promise<MFCC>;


export type { MFCC };
