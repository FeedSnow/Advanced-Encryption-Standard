# Advanced Encription Standard

**AES**, also known as **Rijndael**, is a block cipher based on substitution-permutation network. It has specified size of each block - 128 bits. Algorithm operates on $4\times4$ arrays called *state*. They are made of 16 bytes $b_0, b_1, ..., b_{15}$ ordered in following way:

$$\begin{bmatrix}
b_{0} & b_{4} & b_{8}  & b_{12}\\
b_{1} & b_{5} & b_{9}  & b_{13}\\
b_{2} & b_{6} & b_{10} & b_{14}\\
b_{3} & b_{7} & b_{11} & b_{15}
\end{bmatrix}$$

There are three different sizes of key - 128, 192 or 256 bits. Key size specifies number of transformation rounds that convert the input, called *plaintext*, into the final output, called *ciphertext*:
- for 128-bit keys: 10 rounds
- for 192-bit keys: 12 rounds
- for 256-bit keys: 14 rounds

## High-level description of the algorithm

1. `KeyExpansion`
2. Initial round
    1. `AddRoundKey`
3. Rounds
    1. `SubBytes`
    2. `ShiftRows`
    3. `MixColumns`
    4. `AddRoundKey`
4. Final Round
    1. `SubBytes`
    2. `ShiftRows`
    3. `AddRoundKey`

In step 3 there are $n-1$ rounds where $n$ is number specified by key size as above. The $n$-th round is the final round described in step 4.

## `KeyExpansion`

Key expansion follows certains schedule.
Every $i$-th round has its round constant $rcon_i$:

$$rcon_i = \begin{bmatrix} rc_i & 00_{16} & 00_{16} & 00_{16} \end{bmatrix})$$

where $rc_i$ is an 8-bit value defined as:

$$rc_i = \begin{cases}
1                                   & \quad \text{if } i=1 \\
2 \cdot rc_{i-1}                    & \quad \text{if } i>1 \text{ and } rc_{i-1}<80_{16} \\
(2 \cdot rc_{i-1}) \oplus 11B_{16}  & \quad \text{if } i>1 \text{ and } rc_{i-1} \geq 80_{16} 
\end{cases}$$

## `AddRounKey`

## `SubBytes`

## `ShiftRows`

## `MixColumns`
