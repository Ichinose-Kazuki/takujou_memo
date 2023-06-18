#! /usr/bin/env python
# coding:utf-8
# tcp_server
import socket
import threading

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


bind_ip = '0.0.0.0'
bind_port = 80  # ** 割り当てられたものを使用せよ**
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((bind_ip, bind_port))
server.listen(5)
print('[*] listen %s:%d' % (bind_ip, bind_port))


def arr2sound(arr1, fs, filename="output.wav", volume=1):
    arr1 = arr1 * (32767 * volume / np.abs(arr1).max())
    write(filename, fs, arr1.astype(np.int16))


def gcp_speech2text(filename):
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
        language_code="ja-JP",
        enable_automatic_punctuation=True
    )

    # Detects speech in the audio file
    response = client.recognize(config=config, audio=audio)

    for result in response.results:
        print("Transcript: {}".format(result.alternatives[0].transcript))


def receive(client_socket):
    dataarray = [0]*40000
    count = 0
    bufsize = 4096
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
                dataarray[count+1] = dataarray[count]  # かさ増し
                count += 2
                # print(int(b))
                # print("{:d}\r".format(count), end="")
            except:
                import sys
                print("")
                print(sys.exc_info())
                client_socket.close()
                print('closed')
                print(count)
                return np.array(dataarray)


def receive_and_convert(client_socket):
    dataarray = receive(client_socket)
    # data = []
    # with open('serialOut.log', "r") as f:
    # data.append(int(f.read()))
    # dataarray = np.array(data)
    arr2sound(dataarray, 8000)
    gcp_speech2text("output.wav")


def main():
    # receive_and_convert(0)
    while True:
        client, addr = server.accept()
        print('[*] connected from: %s:%d' % (addr[0], addr[1]))
        client_handler = threading.Thread(
            target=receive_and_convert, args=(client,))
        client_handler.start()

    # a[40000]
    # for i in range(40000):

    # a_2 = a-a.mean()
    # arr2sound(a_2, 8000, "output3.wav", 1)
    # gcp_speech2text("output3.wav")


if __name__ == "__main__":
    main()
