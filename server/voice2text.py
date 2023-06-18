from IPython.display import Audio
import numpy as np
from scipy.io.wavfile import read
from scipy.io.wavfile import write
from matplotlib import pyplot as plt
import os
from google.cloud import speech
# import serial
import io
import time


def arr2sound(arr1, fs, filename="output.wav", volume=1):
    arr1 = arr1 * (32767 * volume / np.abs(arr1).max())
    write(filename, fs, arr1.astype(np.int16))


def gcp_speech2text(filename, lang_select):
    # Instantiates a client
    client = speech.SpeechClient()

    # The name of the audio file to transcribe
    # gcs_uri = "gs://cloud-samples-data/speech/brooklyn_bridge.raw"
    speech_file = filename

    with io.open(speech_file, "rb") as f:
        content = f.read()

    audio = speech.RecognitionAudio(content=content)

    config = speech.RecognitionConfig(
        sample_rate_hertz=8000,
        language_code="en-US" if lang_select == 1 else "ja-JP",
        enable_automatic_punctuation=True
    )

    # Detects speech in the audio file
    response = client.recognize(config=config, audio=audio)

    for result in response.results:
        transcript = result.alternatives[0].transcript
        if transcript != None:
            print("Transcript: {}".format(transcript))
            return transcript

    return response.results[0].alternatives[0].transcript

# @param client_socket socket instance


def receive(client_socket, lang_select):
    dataarray = [0]*40000
    count = 0
    bufsize = 4096

    lang_data = client_socket.recv(1)
    if lang_data == b'j':
        lang_select = 0
        print(lang_data)
    else:
        lang_select = 1
        print(lang_data)
    print(f'lang_select: {lang_select}')

    while True:
        data_chunk = client_socket.recv(bufsize)

        data = [data_chunk[i*4:(i+1)*4] for i in range(int(len(data_chunk)/4))]
        # print(data)
        # data = data.removesuffix(b'/')
        # data = data.removeprefix(b'/')
        # data = data.split(b'/')

        for b in data:
            try:
                dataarray[count] = int(b)
                # count += 1
                if count > 0:
                    dataarray[count-1] = (dataarray[count] +
                                          dataarray[count-2])//2
                # dataarray[count+1] = dataarray[count]  # かさ増し

                count += 2
                # print(int(b))
                # print("{:d}\r".format(count), end="")
            except:
                dataarray[-1] = dataarray[-2]
                import sys
                print("")
                print(sys.exc_info())
                # client_socket.close()
                # print('closed')
                print(count)
                return np.array(dataarray)


def receive_and_convert(client_socket):
    lang_select = -1
    dataarray = receive(client_socket, lang_select)
    # data = []
    # with open('serialOut.log', "r") as f:
    # data.append(int(f.read()))
    # dataarray = np.array(data)
    arr2sound(dataarray, 8000)
    transcript = gcp_speech2text("output.wav", lang_select)
    return transcript
