# Distributed Systems Lab – Assignment 1 (2024-25)

This project implements a **concurrent TCP socket server** in C that interacts with multiple clients to perform distributed computations. The actual computations are delegated to a remote ONC RPC server, using Remote Procedure Calls (RPCs).

## 🧠 Features

Clients can repeatedly request one of the following operations:

1. **Dot Product** of two integer vectors `X` and `Y`  
2. **Average Value** of each vector `X` and `Y`  
3. **Scalar Multiplication** of vector `X` with a real number `r`

All computations are handled by an RPC server. The socket server acts as an RPC client, delegating computation requests and forwarding results to the TCP clients.

## 🏗️ Architecture

- TCP Clients send user input via AF_INET sockets.
- The Socket Server:
  - Accepts multiple client connections concurrently.
  - Acts as a **middleware**, forwarding the client’s request to the RPC server.
- The RPC Server performs the actual computation and returns the result.

## ⚙️ Technologies Used

- C (POSIX Sockets, Threads)
- ONC RPC (`rpcgen`)
- Makefiles

## 🚀 How to Run

### 1. Generate RPC code

Make sure you have `rpcgen` installed.

```bash
rpcgen calculation.x

