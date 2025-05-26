
class SHA256
{
private:
    typedef uint32_t uint32;
    typedef uint8_t uint8;

    const static uint32 K[64];
    uint32 h[8];

    static uint32 rotr(uint32 x, uint32 n)
    {
        return (x >> n) | (x << (32 - n));
    }

    static uint32 ch(uint32 x, uint32 y, uint32 z)
    {
        return (x & y) ^ (~x & z);
    }

    static uint32 maj(uint32 x, uint32 y, uint32 z)
    {
        return (x & y) ^ (x & z) ^ (y & z);
    }

    static uint32 sigma0(uint32 x)
    {
        return rotr(x, 2) ^ rotr(x, 13) ^ rotr(x, 22);
    }

    static uint32 sigma1(uint32 x)
    {
        return rotr(x, 6) ^ rotr(x, 11) ^ rotr(x, 25);
    }

    static uint32 gamma0(uint32 x)
    {
        return rotr(x, 7) ^ rotr(x, 18) ^ (x >> 3);
    }

    static uint32 gamma1(uint32 x)
    {
        return rotr(x, 17) ^ rotr(x, 19) ^ (x >> 10);
    }

    void transform(const uint8 *data)
    {
        uint32 w[64];
        uint32 a, b, c, d, e, f, g, hh, t1, t2;

        for (int i = 0, j = 0; i < 16; ++i, j += 4)
            w[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);

        for (int i = 16; i < 64; ++i)
            w[i] = gamma1(w[i - 2]) + w[i - 7] + gamma0(w[i - 15]) + w[i - 16];

        a = h[0];
        b = h[1];
        c = h[2];
        d = h[3];
        e = h[4];
        f = h[5];
        g = h[6];
        hh = h[7];

        for (int i = 0; i < 64; ++i)
        {
            t1 = hh + sigma1(e) + ch(e, f, g) + K[i] + w[i];
            t2 = sigma0(a) + maj(a, b, c);
            hh = g;
            g = f;
            f = e;
            e = d + t1;
            d = c;
            c = b;
            b = a;
            a = t1 + t2;
        }

        h[0] += a;
        h[1] += b;
        h[2] += c;
        h[3] += d;
        h[4] += e;
        h[5] += f;
        h[6] += g;
        h[7] += hh;
    }

public:
    SHA256()
    {
        reset();
    }

    void reset()
    {
        h[0] = 0x6a09e667;
        h[1] = 0xbb67ae85;
        h[2] = 0x3c6ef372;
        h[3] = 0xa54ff53a;
        h[4] = 0x510e527f;
        h[5] = 0x9b05688c;
        h[6] = 0x1f83d9ab;
        h[7] = 0x5be0cd19;
    }

    void update(const uint8 *data, size_t length)
    {
        for (size_t i = 0; i < length; ++i)
        {
            buffer[buffer_length++] = data[i];
            if (buffer_length == 64)
            {
                transform(buffer);
                buffer_length = 0;
                total_length += 64;
            }
        }
    }

    void final(uint8 *hash)
    {
        uint64_t bit_length = total_length * 8 + buffer_length * 8;

        buffer[buffer_length++] = 0x80;
        if (buffer_length > 56)
        {
            while (buffer_length < 64)
                buffer[buffer_length++] = 0;
            transform(buffer);
            buffer_length = 0;
        }

        while (buffer_length < 56)
            buffer[buffer_length++] = 0;

        for (int i = 0; i < 8; ++i)
            buffer[56 + i] = (bit_length >> (56 - i * 8)) & 0xFF;

        transform(buffer);

        for (int i = 0; i < 8; ++i)
        {
            hash[i * 4] = (h[i] >> 24) & 0xFF;
            hash[i * 4 + 1] = (h[i] >> 16) & 0xFF;
            hash[i * 4 + 2] = (h[i] >> 8) & 0xFF;
            hash[i * 4 + 3] = h[i] & 0xFF;
        }
    }

private:
    uint8 buffer[64];
    size_t buffer_length = 0;
    uint64_t total_length = 0;
};

const uint32_t SHA256::K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

std::string sha256(const std::string &input)
{
    SHA256 sha;
    uint8_t hash[32];

    sha.update((const uint8_t *)input.c_str(), input.length());
    sha.final(hash);

    std::stringstream ss;
    for (int i = 0; i < 32; ++i)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}
