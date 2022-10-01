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

### Round Constants

Key expansion follows certains schedule.
Every $i$-th round has its round constant $rcon_i$:

$$rcon_i = \begin{bmatrix} rc_i & 00_{16} & 00_{16} & 00_{16} \end{bmatrix})$$

where $rc_i$ is an 8-bit value defined as:

$$rc_i = \begin{cases}
1                                   & \quad \text{if } i=1 \\
2 \cdot rc_{i-1}                    & \quad \text{if } i>1 \text{ and } rc_{i-1}<80_{16} \\
(2 \cdot rc_{i-1}) \oplus 11B_{16}  & \quad \text{if } i>1 \text{ and } rc_{i-1} \geq 80_{16} 
\end{cases}$$

where $\oplus$ is the bitwise XOR.

Since there may be up to 10 round constants (explained below), the table below presents values for $rc_i$ in hexadecimal:
<table>
    <tr align="right">
        <th>$i$</th>
        <td>1</td>
        <td>2</td>
        <td>3</td>
        <td>4</td>
        <td>5</td>
        <td>6</td>
        <td>7</td>
        <td>8</td>
        <td>9</td>
        <td>10</td>
    </tr>
    <tr>
        <th>$rc_i$</th>
        <td>01</td>
        <td>02</td>
        <td>04</td>
        <td>08</td>
        <td>10</td>
        <td>20</td>
        <td>40</td>
        <td>80</td>
        <td>1B</td>
        <td>36</td>
    </tr>
</table>
        
### The key schedule

Define:
- $N$ as the length of the key in 32-bit words (4 for AES-128, 6 for AES-192, and 8 for AES-256)
- $K_0, K_1, ..., K_{N-1}$ as the 32-bit words of the original key
- $R$ as the number of needed round keys (11 for AES-128, 13 for AES-192, and 15 for AES-256)
- $W_0, W_1, ..., W_{4R-1}$ as the 32-bit words of the expanded key.

$\mathrm{RotWord}$ is a one-byte left circular shift:

$$\mathrm{RotWord}(\begin{bmatrix}
b_0 & b_1 & b_3 & b_3
\end{bmatrix}) = \begin{bmatrix}
b_1 & b_2 & b_3 & b_0
\end{bmatrix}$$

$\mathrm{SubWord}$ is and application of the AES S-box to each of the four bytes of the word:

$$\mathrm{SubWord}(\begin{bmatrix}
b_0 & b_1 & b_3 & b_3
\end{bmatrix}) = \begin{bmatrix}
\mathrm{S}(b_0) & 
\mathrm{S}(b_1) & 
\mathrm{S}(b_3) & 
\mathrm{S}(b_3)
\end{bmatrix}$$

Then for $i=0 ... 4R-1$:

$$W_i = \begin{cases}
K_i & \quad \text{if } i < N \\
W_{i-N} \oplus \mathrm{SubWord(RotWord}(W_{i-1})) \oplus rcon_{i/N} & \quad \text{if } i \geq N \text{ and } i \equiv 0 (\text{mod } N) \\
W_{i-N} \oplus \mathrm{SubWord}(W_{i-1}) & \quad \text{if } i \geq N, N>6, \text{ and } i \equiv 4 (\text{mod } N) \\
W_{i-N} \oplus W_{i-1} & \quad \text{otherwise.}
\end{cases}$$

## `AddRounKey`

## `SubBytes`

## `ShiftRows`

## `MixColumns`
