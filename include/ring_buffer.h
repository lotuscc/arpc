class RingBuffer {
public:
    RingBuffer();
    int read(int *result);
    int write(int element);

    int read_nbytes(char* buf, int size);
    int write_nbytes(char* buf, int size);

    // int read_allbytes();

private:
    unsigned read_index;
    unsigned write_index;
    static const unsigned buffer_size = 256;
    int buffer[buffer_size];
};

RingBuffer::RingBuffer() : read_index(0), write_index(0) {}

int RingBuffer::read(int *result) {
    unsigned local_read = read_index;
    if (local_read == write_index)
        return 0;
    ++local_read;

    if (local_read == buffer_size)
        local_read = 0;
    *result = buffer[local_read];

    read_index = local_read;
    return 1;
}

int RingBuffer::write(int element) {
    unsigned local_write = write_index;
    ++local_write;
    if (local_write == buffer_size)
        local_write = 0;

    if (local_write != read_index) {
        buffer[local_write] = element;
        write_index = local_write;
        return 1;
    }
    return 0;
}