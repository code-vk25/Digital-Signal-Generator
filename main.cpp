#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <sstream>

using namespace std;
bool alternateAmplitude;
int ext[100] = {0}; // to implement V-violation in B8ZS Scrambling and B00V & 000V in HDB3

// Function to print a substring str[low..high]
void printSubStr(string str, int low, int high)
{
    for (int i = low; i <= high; ++i)
        cout << str[i];

    cout << endl;
}

// Function to find the longest palindromic substring
int longestPalSubstr(string s)
{
    int n = s.length();
    int start = 0, end = 1;
    int low, hi;

    // Traverse the input string
    for (int i = 0; i < n; i++)
    {

        // Find longest palindromic substring of even size
        low = i - 1;
        hi = i;

        // Expand substring while it is palindrome
        // and in bounds
        while (low >= 0 && hi < n && s[low] == s[hi])
        {

            // Update maximum length and starting index
            if (hi - low + 1 > end)
            {
                start = low;
                end = hi - low + 1;
            }
            low--;
            hi++;
        }

        // Find longest palindromic substring of odd size
        low = i - 1;
        hi = i + 1;

        // Expand substring while it is palindrome
        // and in bounds
        while (low >= 0 && hi < n && s[low] == s[hi])
        {

            // Update maximum length and starting index
            if (hi - low + 1 > end)
            {
                start = low;
                end = hi - low + 1;
            }
            low--;
            hi++;
        }
    }

    // Print the longest palindromic substring
    cout << "Longest palindrome substring is: ";
    printSubStr(s, start, start + end - 1);

    // Return output length
    return end;
}

void NRZ_L_function(const vector<bool> &bitStream, sf::VertexArray &waveform, sf::VertexArray &referenceAxis, float periodWidth)
{
    // Encoding parameters for NRZ-L
    const float bitWidth = 50.0f;
    const float bitHeight = 100.0f;
    const float yOffset = 150.0f;
    const sf::Color highColor = sf::Color::Green;
    const sf::Color lowColor = sf::Color::Red;

    float x = 0.0f;

    sf::Vertex axis1(sf::Vector2f(0.0f, yOffset), sf::Color::White);
    sf::Vertex axis2(sf::Vector2f(bitStream.size() * bitWidth, yOffset), sf::Color::White);
    referenceAxis.append(axis1);
    referenceAxis.append(axis2);

    for (float i = 0.0f; i <= bitStream.size() * bitWidth; i += periodWidth)
    {
        sf::Vertex dot1(sf::Vector2f(i, yOffset - 5.0f), sf::Color::White);
        sf::Vertex dot2(sf::Vector2f(i, yOffset + 5.0f), sf::Color::White);
        referenceAxis.append(dot1);
        referenceAxis.append(dot2);
    }

    for (size_t i = 0; i < bitStream.size(); ++i)
    {
        float y = yOffset - (bitStream[i] ? bitHeight : 0.0f);

        sf::Vertex vertex1(sf::Vector2f(x, y), bitStream[i] ? highColor : lowColor);
        sf::Vertex vertex2(sf::Vector2f(x + bitWidth, y), bitStream[i] ? highColor : lowColor);

        waveform.append(vertex1);
        waveform.append(vertex2);

        // Move to the next bit position
        x += bitWidth;

        // Add a vertical line for transitions (0 to 1)
        if (i < bitStream.size() - 1 && !bitStream[i] && bitStream[i + 1])
        {
            waveform.append(sf::Vertex(sf::Vector2f(x, yOffset), highColor));
            waveform.append(sf::Vertex(sf::Vector2f(x, yOffset - bitHeight), highColor));
        }
    }
}

void NRZ_I_function(const vector<bool> &bitStream, sf::VertexArray &waveform, sf::VertexArray &referenceAxis, float periodWidth)
{

    // Encoding parameters for NRZ-I
    const float bitWidth = 50.0f;
    const float bitHeight = 100.0f;
    const float yOffset = 150.0f;
    const sf::Color highColor = sf::Color::Green;
    const sf::Color lowColor = sf::Color::Red;

    sf::Vertex axis1(sf::Vector2f(0.0f, yOffset), sf::Color::White);
    sf::Vertex axis2(sf::Vector2f(bitStream.size() * bitWidth, yOffset), sf::Color::White);
    referenceAxis.append(axis1);
    referenceAxis.append(axis2);

    for (float i = 0.0f; i <= bitStream.size() * bitWidth; i += periodWidth)
    {
        sf::Vertex dot1(sf::Vector2f(i, yOffset - 5.0f), sf::Color::White);
        sf::Vertex dot2(sf::Vector2f(i, yOffset + 5.0f), sf::Color::White);
        referenceAxis.append(dot1);
        referenceAxis.append(dot2);
    }

    float x = 0.0f;
    bool currentLevel = false; // Initialize the current level to low

    for (size_t i = 0; i < bitStream.size(); ++i)
    {
        float y = yOffset - (currentLevel ? bitHeight : 0.0f);

        sf::Vertex vertex1(sf::Vector2f(x, y), currentLevel ? highColor : lowColor);
        sf::Vertex vertex2(sf::Vector2f(x + bitWidth, y), currentLevel ? highColor : lowColor);

        waveform.append(vertex1);
        waveform.append(vertex2);

        // Move to the next bit position
        x += bitWidth;

        if (bitStream[i])
        {
            currentLevel = !currentLevel;
        }
    }
}

void Manchester_function(const vector<bool> &bitStream, sf::VertexArray &waveform, sf::VertexArray &referenceAxis, float periodWidth)
{
    // Manchester encoding logic

    const float bitWidth = 50.0f;
    const float bitHeight = 100.0f;
    const float yOffset = 150.0f;
    const sf::Color highColor = sf::Color::Green;
    const sf::Color lowColor = sf::Color::Red;

    sf::Vertex axis1(sf::Vector2f(0.0f, yOffset), sf::Color::White);
    sf::Vertex axis2(sf::Vector2f(bitStream.size() * bitWidth, yOffset), sf::Color::White);
    referenceAxis.append(axis1);
    referenceAxis.append(axis2);

    for (float i = 0.0f; i <= bitStream.size() * bitWidth; i += periodWidth)
    {
        sf::Vertex dot1(sf::Vector2f(i, yOffset - 5.0f), sf::Color::White);
        sf::Vertex dot2(sf::Vector2f(i, yOffset + 5.0f), sf::Color::White);
        referenceAxis.append(dot1);
        referenceAxis.append(dot2);
    }

    float x = 0.0f;

    for (size_t i = 0; i < bitStream.size(); ++i)
    {
        // Draw the first half of the Manchester bit
        float y1 = yOffset - (bitStream[i] ? bitHeight : 0.0f);
        sf::Vertex vertex1(sf::Vector2f(x, y1), bitStream[i] ? highColor : lowColor);
        sf::Vertex vertex2(sf::Vector2f(x + bitWidth / 2.0f, y1), bitStream[i] ? highColor : lowColor);

        // Draw the second half of the Manchester bit
        float y2 = yOffset - (bitStream[i] ? 0.0f : bitHeight);
        sf::Vertex vertex3(sf::Vector2f(x + bitWidth / 2.0f, y2), bitStream[i] ? lowColor : highColor);
        sf::Vertex vertex4(sf::Vector2f(x + bitWidth, y2), bitStream[i] ? lowColor : highColor);

        waveform.append(vertex1);
        waveform.append(vertex2);
        waveform.append(vertex3);
        waveform.append(vertex4);

        // Move to the next bit position
        x += bitWidth;
    }
}

void Differential_Manchester_function(const vector<bool> &bitStream, sf::VertexArray &waveform, sf::VertexArray &referenceAxis, float periodWidth)
{
    //  Differential Manchester encoding logic

    const float bitWidth = 50.0f;
    const float bitHeight = 100.0f;
    const float yOffset = 100.0f;
    const sf::Color highColor = sf::Color::Green;
    const sf::Color lowColor = sf::Color::Red;

    sf::Vertex axis1(sf::Vector2f(0.0f, yOffset), sf::Color::White);
    sf::Vertex axis2(sf::Vector2f(bitStream.size() * bitWidth, yOffset), sf::Color::White);
    referenceAxis.append(axis1);
    referenceAxis.append(axis2);

    for (float i = 0.0f; i <= bitStream.size() * bitWidth; i += periodWidth)
    {
        sf::Vertex dot1(sf::Vector2f(i, yOffset - 5.0f), sf::Color::White);
        sf::Vertex dot2(sf::Vector2f(i, yOffset + 5.0f), sf::Color::White);
        referenceAxis.append(dot1);
        referenceAxis.append(dot2);
    }

    float x = 0.0f;
    bool lastBit = true; // Initialize to true to ensure a transition at the beginning

    for (size_t i = 0; i < bitStream.size(); ++i)
    {
        bool currentBit = bitStream[i];

        // Draw the first half of the Differential Manchester bit
        float y1 = yOffset - (currentBit ? bitHeight : 0.0f);
        sf::Vertex vertex1(sf::Vector2f(x, y1), currentBit ? lowColor : highColor);
        sf::Vertex vertex2(sf::Vector2f(x + bitWidth / 2.0f, y1), currentBit ? highColor : lowColor);

        // Draw the second half of the Differential Manchester bit
        float y2 = yOffset - (currentBit ? 0.0f : bitHeight);
        sf::Vertex vertex3(sf::Vector2f(x + bitWidth / 2.0f, y2), currentBit ? lowColor : highColor);
        sf::Vertex vertex4(sf::Vector2f(x + bitWidth, y2), currentBit ? highColor : lowColor);

        waveform.append(vertex1);
        waveform.append(vertex2);
        waveform.append(vertex2); // Repeat the middle vertex for synchronization
        waveform.append(vertex3);
        waveform.append(vertex4);

        // Move to the next bit position
        x += bitWidth;

        // Update last bit for the next iteration
        lastBit = !currentBit; // Transition occurs if the current bit is different from the previous bit
    }
}

void AMI_function(vector<bool> &bitStream, sf::VertexArray &waveform, sf::VertexArray &referenceAxis, float periodWidth)
{
    // Encoding parameters for AMI
    const float bitWidth = 50.0f;
    const float bitHeight = 100.0f;
    const float yOffset = 150.0f;
    const sf::Color zeroColor = sf::Color::Yellow; // Zero amplitude level

    sf::Vertex axis1(sf::Vector2f(0.0f, yOffset), sf::Color::White);
    sf::Vertex axis2(sf::Vector2f(bitStream.size() * bitWidth, yOffset), sf::Color::White);
    referenceAxis.append(axis1);
    referenceAxis.append(axis2);

    for (float i = 0.0f; i <= bitStream.size() * bitWidth; i += periodWidth)
    {
        sf::Vertex dot1(sf::Vector2f(i, yOffset - 5.0f), sf::Color::White);
        sf::Vertex dot2(sf::Vector2f(i, yOffset + 5.0f), sf::Color::White);
        referenceAxis.append(dot1);
        referenceAxis.append(dot2);
    }

    float x = 0.0f;
    bool alternateAmplitude = false; // Track the amplitude alternation

    for (size_t i = 0; i < bitStream.size(); ++i)
    {
        bool currentBit = bitStream[i];

        // Determine the amplitude level and color based on the current bit
        sf::Color currentColor;
        float y1, y2;

        if (currentBit)
        {
            if (ext[i] == 1)
            {
                alternateAmplitude = !alternateAmplitude;
            }
            // If the current bit is '1', alternate the amplitude for each '1'
            currentColor = (alternateAmplitude) ? sf::Color::Green : sf::Color::Red;
            if ((alternateAmplitude))
            {
                y1 = yOffset + bitHeight / 2.0f;
                y2 = yOffset + bitHeight / 2.0f;
            }
            else
            {
                y1 = yOffset - bitHeight / 2.0f;
                y2 = yOffset - bitHeight / 2.0f;
            }

            // Invert the amplitude for the next '1'
            alternateAmplitude = !alternateAmplitude;
        }
        else
        {
            // If the current bit is '0', set zero amplitude
            currentColor = zeroColor;
            y1 = y2 = yOffset;
        }

        // Draw the line for the current bit
        sf::Vertex vertex1(sf::Vector2f(x, y1), currentColor);
        sf::Vertex vertex2(sf::Vector2f(x + bitWidth, y2), currentColor);

        waveform.append(vertex1);
        waveform.append(vertex2);

        // Move to the next bit position
        x += bitWidth;
    }
}

void B8ZS_scramble(vector<bool> &bitStream)
{
    // Look for sequences of eight consecutive zeros and replace them with B8ZS violation code
    for (size_t i = 0; i < bitStream.size(); ++i)
    {
        if (i >= 7 && bitStream[i - 7] == false)
        {
            if (bitStream[i - 6] == false)
            {
                if (bitStream[i - 5] == false)
                {
                    if (bitStream[i - 4] == false)
                    {
                        if (bitStream[i - 3] == false)
                        {
                            if (bitStream[i - 2] == false)
                            {
                                if (bitStream[i - 1] == false)
                                {
                                    if (bitStream[i] == false)
                                    {
                                        // Replace eight consecutive zeros with B8ZS violation code
                                        bitStream[i - 4] = true;
                                        ext[i - 4] = 1;
                                        bitStream[i - 3] = true;
                                        bitStream[i - 1] = true;
                                        ext[i - 1] = 1;
                                        bitStream[i] = true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Function to perform HDB3 scrambling
void HDB3_scramble(vector<bool> &bitStream, int &lastViolation)
{
    int consecutiveZerosCount = 0;

    for (int i = 0; i < bitStream.size(); ++i)
    {
        if (bitStream[i] == false)
        {
            consecutiveZerosCount++;

            // Replace every 4th zero with a special sequence
            if (consecutiveZerosCount == 4)
            {
                if (lastViolation % 2 == 0)
                {
                    // B00V Pattern for Even non-zero pulses
                    bitStream[i - 3] = !bitStream[i - 3];
                    bitStream[i] = true;
                    ext[i] = 1;
                }
                else
                {
                    // 000V Pattern for Odd non-zero pulses
                    bitStream[i - 3] = false; // Positive pulse
                    bitStream[i - 2] = false; // Zero
                    bitStream[i - 1] = false; // Zero
                    bitStream[i] = true;
                    ext[i] = 1; // Invert the previous bit
                }

                consecutiveZerosCount = 0; // Reset the consecutive zeros count
            }
        }
        else
        {
            consecutiveZerosCount = 0; // Reset the consecutive zeros count
            lastViolation++;           // Increment violation
        }
    }
}

// Function to perform PCM encoding
void encodePCM(const double *analogSignal, int numSamples, int bitDepth, const string &pcmFileName)
{
    ofstream pcmFile(pcmFileName, ios::binary);

    for (int i = 0; i < numSamples; ++i)
    {
        // Quantize the sample to the specified bit depth
        int quantizedSample = static_cast<int>(round(analogSignal[i] * pow(2, bitDepth - 1)));

        // Write the quantized sample to the PCM file
        pcmFile.write(reinterpret_cast<const char *>(&quantizedSample), bitDepth / 8);
    }

    pcmFile.close();
}

// Function to perform Delta Modulation encoding
void deltaModulation(const double *analogSignal, int numSamples, const string &deltaFileName, string &bitStream)
{
    ofstream deltaFile(deltaFileName, ios::binary);

    // Initial conditions
    double stepSize = 0.1; // Set the step size based on your requirements
    int delta = 0;

    for (int i = 0; i < numSamples; ++i)
    {
        // Calculate the difference between the current sample and the previous transmitted value
        int diff = static_cast<int>(round(analogSignal[i] - delta));

        // Quantize the difference to a single bit
        bool quantizedDiff = (diff >= 0) ? 1 : 0;

        // Update the delta value for the next iteration
        delta += quantizedDiff * stepSize;

        // Write the quantized difference to the Delta Modulation file
        deltaFile.write(reinterpret_cast<const char *>(&quantizedDiff), sizeof(quantizedDiff));

        // Concatenate the quantizedDiff value to the bitStream string
        bitStream += to_string(quantizedDiff);
    }

    deltaFile.close();
}

int main()
{
    // Create a window
    sf::RenderWindow window(sf::VideoMode(1400, 200), "SFML Waveform Example");

    cout << "Enter 0 for Digital input, 1 for analog input : " << endl;
    int choice;
    cin >> choice;

    size_t maxLength = 100;
    string bit_stream_input;

    if (choice == 1)
    {
        // Comment(Vishal) -  Create an analog sgnl based on user input. For this assignment, we will work with sine/cosine waves, as any sgnl can be represented as sum of sine waves [Fourier Transform.]
        int numSamples;
        double amplitude;
        double frequency;
        double sampleRate;
        int bitDepth; // Choose the desired bit depth

        cout << " Enter no. of samples you want to take : (e.g. 100)" << endl;
        cin >> numSamples;
        cout << " Enter Amplitude of signal : (e.g. 200)" << endl;
        cin >> numSamples;
        cout << " Enter frequency of signal : (e.g. 300)" << endl;
        cin >> numSamples;
        cout << " Enter sample rate : (e.g. 600)" << endl;
        cin >> numSamples;
        cout << " Enter the desired bit depth : (e.g. 8/16)" << endl;
        cin >> numSamples;

        double analogSignal[numSamples];

        // Generate a simple sine wave as an example
        for (int i = 0; i < numSamples; ++i)
        {
            analogSignal[i] = amplitude * sin(2 * M_PI * frequency * i / sampleRate);
        }

        cout << "Choose 0 for PCM or 1 for Delta Modulation : " << endl;
        int choice2;
        cin >> choice2;
        if (choice2 == 0)
        {
            // Perform PCM encoding
            encodePCM(analogSignal, numSamples, bitDepth, "output.pcm");

            // Convert PCM to binary stream
            ifstream pcmFile("output.pcm", ios::binary);

            // Read each byte from the PCM file
            while (true)
            {
                char byte;
                pcmFile.read(&byte, 1);

                // Check if the read operation was successful
                if (!pcmFile.good())
                {
                    break; // Break the loop if the read operation fails
                }

                // Convert the byte to 8 bits
                for (int i = 7; i >= 0; --i)
                {   while(bit_stream_input.size() < 500){
                    bit_stream_input += ((byte >> i) & 1) ? '1' : '0';}
                }
            }

           

            // Close the file after reading
            pcmFile.close();

            // Display the binary stream
            cout << "The resultant first 100 bits of bit stream after PCM is (As we tend to get long string of 0s, using AMI with scrambling is recommended for furthur line encoding): " << endl;
            cout << bit_stream_input << endl;
        }

        else if (choice2 == 1)
        {
            // Perform Delta Modulation encoding
            deltaModulation(analogSignal, numSamples, "output_delta.bin", bit_stream_input);

            cout << "The resultant bit stream after DM is (first 8 bits(all will be 1 if signal is monotonically increasing, all 0 if its monotonically decreasing)): " << endl;
            cout << bit_stream_input << endl;
        }
    }

    if (bit_stream_input.length() > maxLength)
    {
        bit_stream_input = bit_stream_input.substr(0, maxLength);
    }

    if (choice == 0)
    {
        // Vector to store the binary data
        cout << "Enter the bit stream: ";

        cin >> bit_stream_input;

        int length = longestPalSubstr(bit_stream_input);
        cout << "length of this longest substring is : " << length << endl;
    }

    vector<bool> bitStream;

    for (char bit : bit_stream_input)
    {
        if (bit == '0')
        {
            bitStream.push_back(false);
        }
        else if (bit == '1')
        {
            bitStream.push_back(true);
        }
        else
        {
            cout << "Invalid Input. Input only 0 and 1 in the bit stream." << endl;
        }
    }
    // Create the waveform
    sf::VertexArray waveform(sf::LinesStrip);
    sf::VertexArray referenceAxis(sf::Lines);

    float periodWidth = 50.0f;

    // Ask the user to select the encoding scheme
    cout << "Select the encoding scheme by choosing one from 1, 2, 3, 4, 5 based on shown encoding(Integral input only):" << endl;
    cout << "1 for NRZ-L\n2 for NRZ-I\n3 for Manchester\n4 for Differential Manchester\n5 for AMI" << endl;

    int encodingScheme;
    cin >> encodingScheme;

    switch (encodingScheme)
    {
    case 1:
        NRZ_L_function(bitStream, waveform, referenceAxis, periodWidth);
        break;
    case 2:
        NRZ_I_function(bitStream, waveform, referenceAxis, periodWidth);
        break;
    case 3:
        Manchester_function(bitStream, waveform, referenceAxis, periodWidth);
        break;
    case 4:
        Differential_Manchester_function(bitStream, waveform, referenceAxis, periodWidth);
        break;
    case 5:
        cout << "Input 0 for AMI without Scrambling, 1 for AMI with B8ZS scrambling, 2 for HDB3 Scrambling :" << endl;
        int a;
        cin >> a;
        if (a == 0)
        {
            AMI_function(bitStream, waveform, referenceAxis, periodWidth);
        }
        else if (a == 1)
        {
            B8ZS_scramble(bitStream);
            AMI_function(bitStream, waveform, referenceAxis, periodWidth);
        }
        else if (a == 2)
        {
            int lastViolation = 0;
            HDB3_scramble(bitStream, lastViolation);
            AMI_function(bitStream, waveform, referenceAxis, periodWidth);
        }
        else
        {
            cout << "wrong input" << endl;
        }
        break;
    default:
        cout << "Invalid choice. Exiting." << endl;
        return 1;
    }

    // Main loop
    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // Clear the window
        window.clear();

        // Draw the refrence axis
        window.draw(referenceAxis);

        // Draw the waveform
        window.draw(waveform);

        // Display the contents of the window
        window.display();
    }

    return 0;
}