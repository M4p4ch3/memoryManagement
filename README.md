
# Memory Management

[![Build Status](https://travis-ci.com/M4p4ch3/memoryManagement.svg?branch=master)](https://travis-ci.com/M4p4ch3/memoryManagement)

## Description

Incomplete memory space management, malloc and free implementation investigation

## Requirements

- `make`
- `gcc`

## Build

```linux
make
```

## Run

```linux
./env.out
```

## Environment Usage

### Alloc

`malloc`, then `<size>`</br>
A memory block of size `<size>` is allocated, associated to an ID, starting with `0`

### Free

`free`, then `<blockID>`</br>
The memory block with ID `<blockID>` is free

### Display

`disp`, for each memory block :</br>

- `<blockID>` : ID</br>
- `<dataSize>` : Size of stored data</br>
- `<memSize>` : Size used in memory (including block header)</br>
- `<allocState>` : Allocation state : `alloc` or `free`</br>

`prettyDisp`, for an user friendly display</br>

### Exit

`-1`

## Run Tests

```linux
sh ./tests.sh
```
