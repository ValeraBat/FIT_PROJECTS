#!/usr/bin/env python
# coding: utf-8

# In[44]:


#get_ipython().run_line_magic('matplotlib', 'notebook')
import numpy as np
import soundfile as sf
import matplotlib.pyplot as plt
from scipy.io import wavfile
from scipy.signal import spectrogram, freqz, tf2zpk
from scipy.signal import find_peaks
from scipy import signal


#Zadaní 1
file, fs = sf.read('xbatyk00.wav', dtype = 'int16') #nacitame file
fmin = file.min()
fmax = file.max()
delka_vzor = file.size
delka_sec = delka_vzor * 1/fs
print("Delka sek: ",delka_sec)
print("Delka vzor:",delka_vzor)
print("Min hod:",fmin)
print("Max hod",fmax)

obraz = plt.figure(figsize = (10,6))
plt.plot(np.arange(delka_vzor) / fs, file)

plt.gca().set_xlabel('$t[s]$')
plt.gca().set_title('Zvukový signál')

plt.gca().grid()
plt.savefig('first_signal.pdf')
plt.close(obraz)


#Zadaní 2

file_mean = np.mean(file)
file = file - file_mean
file_abs = np.abs(file)
file = file / file_abs.max()

obraz = plt.figure(figsize = (10,6))
plt.plot(np.arange(delka_vzor) / fs, file)

plt.gca().set_xlabel('$t[s]$')
plt.gca().set_title('Zvukový signál')

plt.gca().grid()
plt.savefig('second_signal.pdf')
plt.close(obraz)


ramce = []
for i in range(0, file.size, 512):
    ramce.append(file[i:i+1024])
    if ramce[-1].size != 1024:
        ramce[-1] = np.pad(ramce[-1], (0, 1024 - len(ramce[-1])), "constant")


i = 0
for i in range (0, len(ramce) ,1):
    obraz = plt.figure(figsize =(10,6))
    plt.plot(np.arange(i*512,i*512+1024)* 1/fs, ramce[i])
    plt.gca().set_xlabel('$t[s]$')
    plt.gca().set_title('Zvukový signál')
    plt.gca().grid()
    plt.savefig('signal'+str(i)+'.pdf')
    plt.close(obraz)



#Zadani 4
def spectogramm(data,i,fs):
    f,t,srg = spectrogram(data, fs, nperseg = 1024, noverlap = 512)
    srg_log = 10*np.log10(srg+1e-20)
    obraz = plt.figure(figsize=(10,6))
    plt.pcolormesh(t,f,srg_log)
    plt.gca().set_xlabel('Čas [s]')
    plt.gca().set_ylabel('Frekvence [Hz]')
    cbar = plt.colorbar()
    cbar.set_label('Spektralní hustota výkonu [dB]', rotation=270, labelpad=15)
    plt.gca().grid()
    plt.savefig('Spectrogram'+ str(i) +'.pdf')
    plt.close(obraz)

spectogramm(file, 1,fs)

#Zadani 3

ramce_size = ramce[77].size
data = np.matrix(ramce[77]).transpose()
j1, j2 = np.meshgrid(np.arange(ramce_size), np.arange(ramce_size))
exp = np.exp(-2 * 1j * np.pi / ramce_size)
matr = np.power(exp, j1 * j2)
kfs = matr.dot(data)
file_dtf = np.squeeze(np.asarray(kfs))

overim_dft = np.fft.fft(ramce[77])

obraz = plt.figure(figsize=(10, 6))
plt.plot(np.arange(0, fs * 0.5, fs * 0.5 / 512), np.abs(file_dtf[:512]))
plt.grid()
plt.savefig("dft.pdf")
plt.close(obraz)

obraz = plt.figure(figsize=(10, 6))
plt.plot(np.arange(0, fs * 0.5, fs * 0.5 / 512), np.abs(overim_dft[:512]))
plt.grid()
plt.savefig("dft2.pdf")
plt.close(obraz)

#Zadani 5
dft = np.fft.fft(ramce[1])
peaks, _ = signal.find_peaks(np.abs(dft[:512]), height=1)
peaks = 8000*peaks/512
print("Peaks:", peaks)

#Zadani 6
t_cos = np.arange(0,delka_sec,1/fs)
i = 1
cosmix = 0.001*delka_sec*np.cos(2*np.pi*peaks[0]*t_cos)
while i < 4:
    cosmix = cosmix + 0.001*delka_sec*np.cos(2*np.pi*peaks[i]*t_cos)
    i = i + 1

wavfile.write("4cos.wav", fs, cosmix)
fs4, f4cos = wavfile.read('4cos.wav')

spectogramm(f4cos,2,fs4)

#Zadani 7