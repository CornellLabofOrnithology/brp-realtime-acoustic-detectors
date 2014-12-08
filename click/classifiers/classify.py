import numpy as np
    
def init(name):
    state = {}
    params = {'NFFT':512, 'Fs':16000, 'noverlap':384}
    state['params'] = params
    state['win'] = hanning(params['NFFT'])
    return params
    
def classify(data,rate,state):
    # find the time centroid of the data
    params = state['params']
    nfft = params['NFFT']
    data = np.square(data)
    cix = np.floor(mean((data / np.mean(data)) * np.arange(len(data))) - params['NFFT']/2
    cix = np.clip(cix, 0, len(data) - params['NFFT'])

    B = np.absolute(np.rfft(data[cix:cix+nfft] * state['win']))
    f = np.argmax(B)
    f = f * rate / nfft
    if f > 2000:
	score = np.clip(f - 2000, 0, 2000) / 2000
        ret = (1,'hi','hi')
    else:
        score = np.clip(2000 - f, 0, 2000) / 2000
        ret = (1,'lo','lo')
    
    return ret

if __name__ == "__main__":
    import sys
