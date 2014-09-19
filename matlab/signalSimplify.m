function [Y2, x] = signalSimplify(signal, sf)
length = size(signal, 2);
sMin = min(signal);
sMin = min(sMin, 0);
newSignal = signal - sMin;

Fs = sf;                    % Sampling frequency
T = 1/Fs;                     % Sample time
L = length;                     % Length of signal
t = (0:L-1)*T;                % Time vector
% original signal
figure

plot(Fs*t(1:length),signal(1:length));
title('Original Signal visulization')
xlabel('time (milliseconds)')



% original fft
Y = fft(signal);
f = Fs * linspace(0,1,L);
Y_abs = abs(Y);
Y_abs(1,1) = 0;
figure
% Plot single-sided amplitude spectrum.
plot(f, Y_abs, 'x');
%hold on;
title('Original Single-Sided Amplitude Spectrum of y(t)')
xlabel('Frequency (Hz)')
ylabel('|Y(f)|')



figure
% shift fft
%NFFT = 2^nextpow2(L); % Next power of 2 from length of y
Y2 = fft(newSignal);
Y2_abs = abs(Y2);
Y2_abs(1,1) = 0;
%figure
% Plot single-sided amplitude spectrum.
plot(f, Y2_abs, 'r.') 
title('shifted Single-Sided Amplitude Spectrum of y(t)')
xlabel('Frequency (Hz)')
ylabel('|Y(f)|')





% reconstruct signal

x = ifft(Y2);
x = abs(x) + sMin;

figure
plot(Fs*t(1:length), x) 
title('new Signal visulization')
xlabel('time (milliseconds)')

end

