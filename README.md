### Implementation of digital signal generator <hr>
<ol>
  <li>Line coding schemes implemented: NRZ-L, NRZ-I, Manchester, Differential Manchester, AMI and scrambling schemes: B8ZS, HDB3</li>
  <li>Pulse code modulation (PCM) or Delta modulation (DM)</li>
</ol>

### Input <hr>
<p>
Select whether you want to input a digital or an analog signal.
For digital signal : </br>
For encoding, select an option (NRZ-L, NRZ-I,
Manchester, Differential Manchester, AMI, Scrambling). </p>
  <p>
  For PCM/DM, choose a  technique, then the digital data generated can
be fed to one of line encoding techniques.</p>

### Output <hr>
<p>
Digital data stream given, longest palindrome in that data stream,
digital signal produced and in case of scrambling, scrambled signal produced.</p>

### Language and Libraries Used: <hr>
• Language: C++ </br>
• Libraries: SFML Graphics Library

### How to Run the Code: <hr>
#### Instructions for Linux:
1. Install SFML graphics library</br>
     In terminal, run command: </br>
     `sudo apt install libsfml-dev`  </br>
2. Locate location of SFML library:  </br>
    In terminal, run command:  </br>
    `locate libsfml`  </br>
    Expected output:  </br>
    `/usr/lib/x86 64-linux-gnu/libsfml`  </br>
     If your sfml library is stored at a different address, update the address in 
     the following commands.
3. In VSCode, open the terminal and execute these commands in the same
    directory as the source code file:  </br>
   (a) Compile:  </br>
     `g++ -c main.cpp -I"/usr/lib/x86 64-linux-gnu/libsfml"/include`  </br>
     // replace main.cpp with the name of your C++ file  </br>
   (b) Final Executable:  </br>
    ` g++ main.o -o sfml-app -L"/usr/lib/x86 64-linux-gnu/libsfml"/lib-lsfml-graphics -lsfml window -lsfml-system`  </br>
    // replace main.o with the new .o file created  </br>
    // replace sfml-app with a new name for a new executable  </br>
   (c) Execute the compiled program:  </br>
    `export LD LIBRARY PATH="/usr/lib/x86 64-linux-gnu/libsfml"/lib && ./sfml-app`  </br>
     // replace sfml-app with the name of the executable created  </br>
4. This will open an SFML graphics window. Use the VSCode terminal to
    input the bit stream or the analog signal and select the encoding scheme
     you want. Then observe the SFML window for the resultant digital signal
    waveform. In case of digital input, the length of the longest palindrome
    and the longest palindrome will be displayed in the terminal right after
    the user inputs the bit stream.
### Assumptions: <hr>
<p>
It has been assumed that in case of analog to digital
conversion, the analog signal will be a sine wave. As Fourier transform can be
used to convert any signal to a combination of sine waves, this assumption
allows any analog signal to be converted. The program asks for the Amplitude,
frequency, sampling rate (ideally, more than twice the highest frequency), and
no. of samples and bit depth to perform PCM/DM modulation. After the
PCM/DM encoding, the bit string is stored in a string bit stream input, which
is restricted to 500 bits to make the program comprehensible to the user. In case
the user wants to remove the 500-bit limit, the while loop on line 538 needs to
be removed. The program outputs the bit string to show the user the resultant
PCM/DM digital string.
</p>
<p>
For digital signals, no such assumptions have been taken. Any bit stream of
any length can be given as input, and any line encoding scheme can be implemented. Note that for Scrambling in AMI, a boolean value named alternateAmplitude
and an array of size 100 named ext[100] is used. For a very long bit stream,
the use of a dynamic array is suggested. However, for all practical testing
purposes, these variables should be adequate.</p>
