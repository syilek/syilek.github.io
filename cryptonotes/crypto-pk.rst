********************************************************************
Key Exchanges, Public-Key Encryption, and Digital Signatures
********************************************************************

Key Management
====================


So far we have seen strong cryptographic tools (encryption, MACs) that require Alice and Bob 
to share the same key.  An obvious question is how can Alice and Bob establish this shared key? 
One way would be for Alice and Bob to schedule an in-person meeting (maybe in a dark alley somewhere)
and agree on a shared key to use.  This type of key exchange might be practical amongst friends or 
even inside of a small company, but in general is not a very good solution.

The number of keys also becomes an issue.  Imagine you have a company with 100 employees.  Each employee 
needs to be able to securely communicate with every other employee.  Thus, if Joe, Dave and Liz are employees, 
Joe would need a shared key with Dave (call it :math:`K_{JD}`) but also one with Liz (:math:`K_{JL}`).  With 100 employees, each 
employee would share a key with the 99 other employees.  This leads to 100*(99)/2 = 4950 keys!  If an employee 
accidently reveals her keys, then she will have to re-key with all 99 other employees.  If we increase the 
size of the company to 1000 employees, we end up needing almost 50 thousand keys!


Key Distribution Centers (KDCs)
---------------------------------
One solution to this problem is to rely on **Key Distribution Centers (KDCs)**.
A key distribution center is a trusted party that shares keys with everyone in some group (e.g., a company).  
If Joe wants to talk to Dave, then Joe, who shares a key :math:`K_J` with the KDC, sends the KDC a message saying 
"I want to talk securely to Dave".  
The KDC generates a new random session key :math:`K_{JD}` that Joe and Dave can use for a limited period of time.  
The KDC then sends Joe :math:`\mathtt{enc}(K_J, K_{JD}), \mathtt{enc}(K_D, K_{JD})`.  Joe can retrieve :math:`K_{JD}` 
and also send the second ciphertext over to Dave, who can use :math:`K_D` to also retrieve :math:`K_{JD}`.

This type of framework is often used inside of organizations (companies, Universities).  You may have heard of Kerberos, 
a popular variant of this. What are the disadvantages of this scenario? For one, there is a single point of failure (the KDC) that must always be available, or "online".  Second, the KDC must be constantly online or no one can communicate.  Third, the KDC knows all of the keys, so it can read all of the encrypted messages and must be completely trusted.  This makes it an attractive target as well.

We will explore "offline" alternatives to KDCs that use new cryptographic objects called asymmetric primitives.



A Revolution: Asymmetry in Cryptography
=========================================

In their 1976 paper "New Directions in Cryptography", Diffie and Hellman 
described key exchange, public-key encryption, and digital signatures.  These three 
tools would allow people to use cryptography without sharing secret keys in advance.

Let's start with key exchange.  Key exchange is a very general term that can apply 
to any interaction between Alice and Bob that results in the two of them sharing a key.
The minimum security property a good key exchange scheme should have is that an adversary 
observing this interaction shouldn't be able to learn the key.

How could such a thing even be possible?  We'll try to give some intuition by describing 
a simple (but not secure) key exchange scheme.  The ideas in this scheme can be used 
to build other more secure schemes.

Imagine there is a public 5x5 matrix :math:`G`.  As an example we'll use

.. math::

   G =
   \left(
   \begin{array}{ccccc} 
   101 & 55 & 72 & 124 & 78\\
   15 & 941 & 3 & 179 & 220\\
   91 & 315 & 423 & 663 & 307\\
   312 & 200 & 300 & 17 & 777\\
   408 & 97 & 818 & 911 & 856
   \end{array}
   \right)

Since :math:`G` is public, both Alice and Bob have access to it.  

Key exchange then proceeds as follows:
   
   #. Alice randomly chooses some rows from the matrix.  To do so, you can imagine she flips a coin for each row.  If the coin lands on heads, she includes that row, and otherwise ignores it.  Once she has a subset of the rows chosen, she sums each column, only using the rows she chose.  She sends these 5 values to Bob.  For our example, let's say Alice chooses rows 1,3, and 4.  She would sum up the columns and get 504, 570, 795, 804, 1162.


   #. Bob does something similar.  He randomly chooses a subset of columns and sums each row (using only the chosen columns). He sends these 5 values to Alice. For our example, let's say Bob chooses columns 1,2, and 3.  He would get sums 228, 959, 829, 812, 1323.

   #. Alice treats Bob's values as a column and again adds up the rows she chose.  In our example, these were rows 1,3, and 4.  So she gets 228+829+812 = 1869.  Similarly, Bob treats Alice's values as a row vector and adds up the columns he chose.  In our example, these were rows 1,2, and 3.  So he gets 504+570+795 = 1869.

   #. Alice and Bob now share a common value (1869 in our example).  They can use this directly as a key or derive another key from it using perhaps a hash function to map it into bitstrings of a certain length (which may be more convenient keys).


Now, the scheme we just described would not be secure, since it wouldn't be very hard for an 
adversary to figure out the chosen rows and columns.  But, imagine if the matrix were 
300x300 and each element of the matrix was a carefully-chosen 300-bit number.  You can 
start to see that it might become difficult for an adversary to learn anything about 
the key from observing the sums Alice and Bob compute.


Public-Key Encryption
======================

Consider again the scenario in which Alice wants to securely communicate with Bob.
Public-key Encryption (PKE), sometimes called Asymmetric Encryption, 
allows anyone to encrypt a message for Bob, but only allows Bob to decrypt.  
Specifically, Bob will generate a public key :math:`\mathit{pk}_B` and a 
corresponding secret key :math:`\mathit{sk}_B`.  Bob can then make 
his public key public, meaning Alice will know it, but so will Eve.  Bob
keeps the secret key to himself. 

If Alice wants to encrypt a message for Bob, she runs an encryption program 
:math:`\mathcal{E}`, with inputs Bob's public key :math:`\mathit{pk}_B` 
and the message :math:`M`, to generate a ciphertext :math:`C`.  Alice 
sends this ciphertext to Bob.  Upon receiving the ciphertext, Bob 
runs the decryption program :math:`\mathcal{D}`, with inputs the secret 
key :math:`\mathit{sk}_B` and the ciphertext :math:`C`, to recover the original 
message.  It should be the case that if :math:`C = \mathcal{E}(\mathit{pk}, M)`, 
then :math:`\mathcal{D}(\mathit{sk}, C)` outputs :math:`M`.

The question, of course, is how can we build such an encryption scheme.  

RSA
----

The RSA algorithm is a deterministic public-key encryption scheme (sometimes formally called a trapdoor permutation) invented in 1977 by Ron Rivest, Adi Shamir, and Leonard Adelman.  RSA can be used as a building block to create practical public-key encryption schemes.  Let's look at some details of how RSA works.

To generate RSA public and secret keys, Bob should choose two large prime numbers 
:math:`p` and :math:`q`.  How large should they be?  1000 or more bits is desirable.  To get a sense of how large a 1000 bit number is, we can use Python to see what 2 to the 
power 1000 is in decimal::

   $ python
   ...
   >>> 2**1000
   10715086071862673209484250490600018105614048117055336074
   43750388370351051124936122493198378815695858127594672917
   55314682518714528569231404359845775746985748039345677748
   24230985421074605062371141877954182153046474983581941267
   39876755916554394607706291457119647768654216766042983165
   2624386837205668069376L

Bob should then multiply :math:`p` and :math:`q` to get a value we will call :math:`N`.
Next, Bob should choose an :math:`e` that is relatively prime to :math:`(p-1)(q-1)` and find :math:`d` such that :math:`ed \equiv 1 \mod (p-1)(q-1)`. (The value :math:`(p-1)(q-1)`, 
sometimes referred to as :math:`\varphi(N)`, is the number of values relatively 
prime to :math:`N`.)
The public key is then :math:`(e,N)` and the secret key is :math:`(p,q,d)`.

.. note::
   How can we generate a 1000 bit prime number?  Since we know how to quickly test primality, the best way is to use rejection sampling.  Generate a random 1000 bit number, and then test if it is prime.  If so, we are done.  If not, we simply generate another random 1000 bit number, and test if it is prime.  We continue in this way until we finally find a prime.  It turns out that because of the density of prime numbers, this procedure will terminate relatively quickly.

To encrypt a message :math:`M` with public key :math:`(e,N)`, one interprets 
the bits of :math:`M` as the binary representation of a number, and computes 
:math:`C = M^e \mod N`.

To decrypt a ciphertext :math:`C` with secret key :math:`(p,q,d)`, simply 
compute :math:`C^d \mod N`.  Since :math:`e` and :math:`d` are inverses of each 
other, 

.. math::
   :nowrap:

   \begin{eqnarray*}
   C^d \mod N &=& (M^e)^d \mod N\\
   &=& M^{ed} \mod N\\
   &=& M^1 \mod N\\
   &=& M
   \end{eqnarray*}

and we recover the original message.

-------------------------------------------------

Hands-on: RSA in Python
^^^^^^^^^^^^^^^^^^^^^^^^^

To see the basics of RSA in action, let's try a toy example in Python
with :math:`p=29` and :math:`q=31`::

   $ python
   ...
   >>> p = 29
   >>> q = 31
   >>> N = p*q

Now, continuing in Python, we choose :math:`e` as 11 and we need to compute 
:math:`d`.  While there are good ways to do this using the Extended Euclidean Algorithm, for our purposes we can simply employ a loop 
when using such small values::

   >>> e = 11
   >>> for i in range(1, (p-1)*(q-1)):
   ...     if ((e*i) % ((p-1)*(q-1)) == 1):
   ...             print i
   ... 
   611
   >>> d = 611

So :math:`d` should be 611.  Now, to encrypt a message 97::

   >>> M = 97
   >>> C = M**e % N
   >>> C
   872L

So the ciphertext is 872.  We can make sure decryption works::

   >>> C**d % N
   97L

which it does.
 
-------------------------------------------

For the rest of this chapter, we will refer to the RSA encryption scheme 
described above as *plain RSA* (sometimes called "textbook RSA").
One common misconception is that plain RSA, by itself, is a good public key encryption scheme.

Plain RSA is Insecure
----------------------

The truth is that plain RSA is an insecure public key encryption scheme.  There are many reasons for this, but the one we will focus on is that it is *deterministic*.  By deterministic, we mean that if the encryption program is run multiple 
times on the same inputs, it always gives the same outputs.  

To see why this is a problem, consider a scenario 
in which Alice is sending encrypted messages to her stock broker using a deterministic 
public key encryption scheme like plain RSA.  The broker has public key :math:`\mathit{pk}_B` and 
secret key :math:`\mathit{sk}_B`.  Further, suppose Alice is either encrypting the message "buy" or the message "sell", and 
that the adversary knows these are the two possible messages.  
Alice decides to buy a stock, so she computes :math:`C=\mathcal{E}(\mathit{pk}_B, \textrm{buy})` and sends 
:math:`C` to the broker.  

It turns out that in this situation, the adversary can easily determine which message was encrypted.  Because 
we have a public key encryption scheme, Eve knows the broker's public key :math:`\mathit{pk}_B`.  So Eve 
can compute :math:`C_0 = \mathcal{E}(\mathit{pk}_B, \textrm{buy})` and :math:`C_1 = \mathcal{E}(\mathit{pk}_B, \textrm{sell})`
and compare :math:`C` to :math:`C_0` and :math:`C_1`.  Since the encryption scheme is deterministic, :math:`C` will equal 
:math:`C_0`, and the adversary will know that "buy" was the encrypted message.




Randomized Encryption
----------------------

It might seem difficult to overcome the problems with deterministic public key encryption described in the previous section, since most functions we write are deterministic.  What does a function that is not deterministic even look like?  We will focus on encryption functions that generate random values and change their behavior depending on these values.  We will call the resulting schemes randomized public key encryption schemes. 

Before giving examples, let's discuss why this would help in the buy/sell example from the previous section.  One way to think of a randomized encryption function is having it take an additional random input, :math:`r`.  So in the stock example, 
when Alice wants to encrypt "buy", she generates a long random bit string :math:`r` and computes :math:`C=\mathcal{E}(K, \textrm{buy}, r)`.  Notice the random value is a third argument to the encryption routine.  Eve sees the ciphertext :math:`C`, but she now has a much more difficult task.  She cannot simply compute an encryption of "buy" and an encryption of "sell" and compare the ciphertexts to :math:`C`, since she does not know what random input to use for the third argument.  Thus, the attack 
does not work.

.. note:: 
   We can already see from this example that the randomness used by the encryption function needs to be sufficiently long, or an attack is still possible.  For example, if the encryption routine only uses a 3 bit random number, then there are only 8 possible random inputs, and Eve can easily try all of them in a small amount time. 



How can we randomize RSA?  One straightforward idea is to paste the randomness onto the front of the message and encrypt the resulting value.  This is called **Padded RSA**.  Specifically, :math:`\mathcal{E}` takes a sufficiently long random value 
:math:`r` and encrypts message :math:`M` with RSA public key :math:`(e, N)` by computing

.. math::
   C = (r||M)^e \mod N 

The PKCS #1.5 standard for RSA encryption proposes a variant of padded RSA with a slightly more 
complicated padding scheme.  This standardized process is widely-used in practice, and is believed to be 
secure.  However, no formal proof of this exists.



Hybrid Encryption and Long Messages
---------------------------------------

In the previous section, we saw that we can randomize RSA by padding the message with a random value before encrypting.  While this seems to work well in practice, there are still some issues.  One issue is how to encrypt long messages with RSA.  If the modulus :math:`N` is 2048 bits, which is fairly typical, the message cannot be longer than 2048 bits, and we also need to leave room for the random padding.  What if we want to use RSA to encrypt a message that is millions of bits long?  One option is to split the message up into smaller blocks and encrypt each separately with padded RSA.  This is much like ECB mode for blockciphers, but the random padding prevents patterns from emerging like with ECB.  The problem with this is that public key cryptography is very inefficient.  We are doing non-trivial mathematical operations on extremely large numbers.

Thus, instead of using RSA to encrypt each chunk of a long message, we can instead use what's typically called **hybrid encryption**.  Hybrid encryption is a hybrid of public key and symmetric key encryption in which the public key encryption routine is used to encrypt a single random value.  This random value can then be used to derive keys for symmetric encryption, which is what is actually used to encrypt the message.
Let's look at this idea in more detail with RSA.  Let :math:`H` be a hash function, and let :math:`\mathtt{enc}` be a symmetric encryption scheme.
We can build a new public key encryption function :math:`\mathcal{E}` that uses an RSA 
public key :math:`(e,N)` to encrypt a long message :math:`M` as follows:

   #. Choose a random value :math:`r` and compute :math:`C_1 = r^e \mod N`.
   #. Hash the random value to generate a symmetric key :math:`K`.  Specifically, let :math:`K = H(r)`. 
   #. Encrypt :math:`M` with the symmetric encryption scheme and key :math:`K`, giving us :math:`C_2 = \mathtt{enc}(K, M)`.
   #. The ciphertext is the pair :math:`(C_1, C_2)`.


One can decrypt :math:`(C_1, C_2)` with :math:`(d,p,q)` by first computing :math:`r = C_1^d \mod N`, hashing :math:`r` to get :math:`K = H(r)`, and then using the symmetric decryption program with :math:`K` to learn the message :math:`M = \mathtt{dec}(K, C_2)`.

This scheme has the advantage that only one message (the random value) needs to be encrypted with RSA, while the long message actually gets encrypted with a symmetric encryption scheme.  Since symmetric encryption tends to be much faster than public-key encryption, we get a huge efficiency gain.

We also appear to get a security gain, since this scheme, often called RSA-KEM (KEM stands for "Key Encapsulation Mechanism") can be proven secure assuming our hash function acts like a public random table.


RSA and Factoring
-------------------

It is often stated that the security of RSA is based on factoring large numbers.  It is important to understand exactly what this means.  

The problem of factoring a large composite number into its prime factors has long been studied in mathematics and computer science.  While a lot of progress has been made on the problem, the best algorithms are still much too inefficient to use on the large numbers we use for RSA.  In computational complexity terminology, we would say that we do not have a polynomial time algorithm for factoring. 

So what does it mean for the security of RSA to be *based* on factoring?  Let's first explore why the following is true:

   If factoring is easy, then RSA is insecure.

This is true because if factoring is easy, then an adversary can take :math:`N` from the public key and find the prime factors :math:`p,q`.  From these, it is easy to find :math:`d`, which can be used for decryption.  

.. note::
   If you are familiar with logic, you might know that the contrapositive of an implication is also true.  This means that "If A, then B" being true also leads to "If not B, then not A" being true.  The result for RSA is that the following is true:

      If RSA is secure, then factoring is hard.

It it very important to note that the *converse* is not necessarily true.  (The converse would be "If factoring is hard, then RSA is secure.")     We do not know if this is the case.  It could very well be that factoring is hard, but there is some other way to break RSA that does not involve factoring.












Digital Signatures
====================

The last public key tool we will discuss are **Digital Signatures**, which are the 
public key analogue of MACs and provide authentication and integrity, and have some other 
useful additional properties.

A digital signature scheme has two main algorithms.  The :math:`\mathtt{Sign}` program 
takes as input a secret signing key :math:`\mathit{sk}` and a message :math:`M` and outputs 
a signature :math:`S`.  The verification program :math:`\mathtt{Vf}` takes as input a public 
verification key :math:`\mathit{vk}`, a message :math:`M`, and a signature :math:`S`, and returns 
either *accept* or *reject*, depending on whether or not :math:`S` is a valid signature 
for :math:`M`. 

As we shall see, digital signatures are extremely important for e-commerce.  But for now, let us 
focus on a simpler scenario in which Alice wishes to send Bob an important email, call it :math:`A`.  She wants Bob 
to be certain that the message he receives is from her, and has not been tampered with in any way 
while in transit.  If Alice and Bob shared a symmetric key, she could use a MAC.  But in our current scenario, 
let us assume that Alice and Bob do not share a key.  Instead, Alice has a secret signing key 
:math:`\mathit{sk}` and the corresponding public verification key :math:`\mathit{vk}`.  Since the verification 
key is public, we can assume both Bob and the adversary, Eve, have access to it.

Alice signs her email by computing :math:`S = \mathtt{Sign}(\mathit{sk}, A)` and sends both the email :math:`A`
and the signature :math:`S` to Bob.  Bob, upon receiving :math:`(A, S)`, uses Alice's public verification key 
to verify the signature by computing :math:`\mathtt{Vf}(\mathit{vk}, A, S)`.  If the verification algorithm 
outputs *accept*, Bob trusts the email; if it outputs *reject* he throws out the email.


A digital signature scheme also gives us a couple of additional properties:

   #. **Public-verifiability**. Anyone can verify a signature given the public verification key.  
   #. **Non-repudiation**.  The signer cannot later claim she did not create the signature. 


Similar to MACs, with digital signatures, the adversary's goal is to generate a forgery.  In the signature 
setting, forging means that adversary, after observing some number of messages and the corresponding valid signatures, is able to 
generate a valid signature for a new message. 


RSA Signatures
---------------

How can we build a digital signature scheme?  Clearly, we cannot simply attach an image of a handwritten signature to an email, since 
that would make it easy for an adversary to copy the image and attach it to other, fake messages.  It turns out 
we can use RSA to build digital signatures, but we need to be careful.
Many basic resources on cryptography say that Plain RSA can be used for public key 
encryption and Plain RSA "in reverse" can be used for digital 
signatures.  This is wrong and dangerous.  We have already seen that Plain RSA, by itself, is a terrible PKE scheme.  Similarly, we 
will see that Plain RSA "in reverse" is, by itself, also a terrible digital signature scheme. 
Let's examine what leads to this misunderstanding.

First, let's try to use Plain RSA "in reverse" as a signature scheme.  The public verification key will be :math:`\mathit{vk}=(e,N)` and the secret signing key will be :math:`\mathit{sk}=(d,p,q)`.  If Alice is the one signing, she will 
compute :math:`S=\mathtt{Sign}(\mathit{sk}, M) = M^d \mod N` (since we are taking a message to the :math:`d` instead of :math:`e`, this is where the "in reverse" comes from).  The verification program :math:`\mathtt{Vf}(\mathit{vk}, M, S)` 
will *accept* if :math:`M = S^e \mod N` and *reject* otherwise. 

In terms of functionality, the scheme works for the same reason decryption works in Plain RSA encryption.  Unfortunately, 
it is easy for an adversary to generate forgeries.  In fact, the verification algorithm can actually be used to create forgeries!
Before we give the details of the attack, let's recall what constitutes a forgery.  A forgery is a pair :math:`(M^*, S^*)` produced 
by the adversary
such that :math:`S^*` is a valid signature for :math:`M^*` and :math:`M^*` is a message for which the adversary has not previously 
seen a valid signature.   In other words, after seeing some number of messages and their signatures, the adversary forges if 
she can come up with some new message and a corresponding signature.  An important point is that :math:`M^*` can be *any* message
and still count as a forgery, even if it is simply a random string of bits.

Now, returning to Plain RSA signatures, the attack is simple.  The adversary first picks the *signature* :math:`S^*` and then 
computes the corresponding *message* :math:`M^* = (S^*)^e \mod N`.  It is easy to see that :math:`(M^*, S^*)` is a valid 
message-signature pair.   
We have now seen that Plain RSA is bad for encryption and Plain RSA in reverse is bad for signatures, so it is best to dispense 
with the notion that signatures are just public-key encryption in reverse.


Just as we were able to fix the problems with Plain RSA in the encryption setting, we will be able to repair the above RSA signature scheme.  The idea is to hash the message before using RSA.  The resulting signature scheme is commonly called **Hashed RSA**.  In detail, 
to generate a hashed RSA signature using secret key :math:`\mathit{sk}=(d,p,q)` and hash function :math:`H`, compute 
:math:`\mathtt{Sign}(\mathit{sk}, M) = H(M)^d \mod N`.  The verification program :math:`\mathtt{Vf}(\mathit{pk}, M, S)` returns 
*accept* if :math:`S^e \mod N == H(M)` and *reject* otherwise.

Note that the attack on plain RSA signatures no longer works on hashed RSA, since it would involve the adversary inverting the
hash function.  If an adversary could invert a hash function, she could find collisions, breaking the main security property of 
the hash function. 

Unfortunately, there all still issues with Hashed RSA as described above.  The output of a typical hash function 
is only a few hundred bits (e.g., SHA-1 has 160 bit outputs), while a typical RSA modulus might be 2048 bits.   
For a number of reasons beyond the scope of this course, it is more desirable to apply RSA to a value much closer in quantity 
to the modulus.  Thus, practical RSA signature schemes apply padding before applying RSA.  For example, to use 
RSA signatures with an :math:`n` byte modulus and a hash function :math:`H` with :math:`h` byte outputs, we can use the standardized 
PKCS #1.5 padding and generate the value :math:`Y=` 00 || 01 || FF || ... || FF || 00 || :math:`H(M)`, 
with :math:`n-h-3` FF bytes.  Then the signature is :math:`Y^d \mod N`.  To be more concrete, suppose we wish to sign 
the message "hello" using the SHA-1 hash function and our RSA modulus is 1024 bits (128 bytes).  
Then the 128 bytes we would actually apply RSA to are::

   0001ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
   ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
   ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
   ffffffffffffffffffffff00aaf4c61ddcc5e8a2dabede0f3b482cd9aea9434d

Notice the last 20 bytes are the result of hashing "hello" (no newline) with SHA-1.

There are a number of other padding schemes commonly used with RSA signatures.  The most secure option is probably to 
use RSA-PSS, which has the most theoretical backing.

Distributing Public Keys
==========================

We end this chapter with a brief discussion on distributing public keys.  Suppose that Alice wants to send encrypted 
messages to Bob using his public key.  In the above discussions, we simply assumed that Alice and Eve know Bob's public 
key because it's *public*. But this is a significant oversimplification.  If Alice and Bob are communicating over the Internet, 
Bob cannot simply send his public key to Alice, since there is the potential for a **Man-in-the-middle Attack (MITM)**.  In 
such an attack, Eve intercepts Bob's public key and replaces it with her own public key.  Alice receives Eve's public key, 
thinking it is actually from Bob!

To prevent MITM attacks, we need some way for Alice to verify that the public key she receives is actually from Bob.  He could use a digital signature scheme to authenticate his public key, but then Alice would need his public key to verify the signature.  It seems like 
a "chicken and egg problem", but we will soon see a way to solve it.
 









