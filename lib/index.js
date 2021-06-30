import { mfccInit, MFCC_DEFAULTS, WINDOW_FUNCTION } from "./mfcc.js";


const ISNODE = typeof process === "object" && typeof process.versions === "object" && typeof process.versions.node === "string";


export default async function(options) {
	const uri = new URL("../dist/mfcc.wasm", import.meta.url);
	if (!ISNODE) {
		return fetch(uri.toString()).then((response) => response.arrayBuffer()).then((BINARY) => mfccInit(BINARY, options));
	}
	const fs = await import("fs");
	const buffer = fs.readFileSync(uri.pathname);
	return mfccInit(buffer, options);
}


export { MFCC_DEFAULTS, WINDOW_FUNCTION };
