#! /usr/bin/env python
# coding:utf-8
# tcp_server
import socket
import threading

from voice2text import receive_and_convert
from text2img_linux import convert_to_image


bind_ip = '0.0.0.0'
bind_port = 10000  # ** 割り当てられたものを使用せよ**
server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server.bind((bind_ip, bind_port))
server.listen(5)
print('[*] listen %s:%d' % (bind_ip, bind_port))


def handle_client(client_socket):
    transcript = receive_and_convert(client_socket)
    im_send = convert_to_image(transcript)
    byte_list = list(map(lambda x: x.to_bytes(1, "big"), im_send))
    byte_send = b''.join(byte_list)

    client_socket.send(b'\x01')  # データが送られ始めることを知らせる
    # for i in byte_send:
    #     if i != b'\xff':
    #         print(i)

    sent = client_socket.send(byte_send)
    print(sent)
    client_socket.close()
    print('closed')


def main():
    while True:
        client, addr = server.accept()
        print('[*] connected from: %s:%d' % (addr[0], addr[1]))
        client_handler = threading.Thread(
            target=handle_client, args=(client,))
        client_handler.start()


if __name__ == "__main__":
    main()
