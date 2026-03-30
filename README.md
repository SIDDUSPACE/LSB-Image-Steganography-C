# 🔐 LSB Image Steganography in C

> A command-line C project to **hide and retrieve secret text inside BMP images** using the **Least Significant Bit (LSB)** technique.

---

## 🌟 Project Overview

This project implements **image steganography** by embedding secret text into the **pixel bytes of a BMP image** without causing visible distortion.

### ✨ Core Features

* ✅ Encode secret text into a BMP image
* ✅ Decode hidden text from stego image
* ✅ Magic string validation
* ✅ Capacity checking before encoding
* ✅ Preserves BMP header and image integrity
* ✅ Negative test support (magic string mismatch)

---


## ⚙️ Build & Run

### 🔨 Compile

```bash
gcc src/*.c -Iinclude -o stego
```

### 🔐 Encode

```bash
./stego -e test/beautiful.bmp test/secret.txt output/output.bmp
```

### 🔓 Decode

```bash
./stego -d output/output.bmp output/decoded.txt
```

### 🧪 Verify

```bash
cat test/secret.txt
cat output/decoded.txt
```

---

## 🖼️ Execution Flow (Visual)

```text
Input BMP + Secret Text
          │
          ▼
   Capacity Check
          │
          ▼
   Copy BMP Header (54B)
          │
          ▼
   Encode Magic String
          │
          ▼
   Encode File Size
          │
          ▼
   Encode Secret Data
          │
          ▼
   Generate Stego BMP
          │
          ▼
      Decode Back
          │
          ▼
   Verify Original = Decoded
```

---

## 📸 Output Screenshots (Execution Results)

Add your screenshots in the `Execution_Results/` folder using this exact order:

### 1️⃣ Encoding Execution

`01_Encoding_Execution_Output.png`

### 2️⃣ Output Folder Validation

`02_Output_Folder_File_Check.png`

### 3️⃣ Decoding Execution

`03_Decoding_Execution_Output.png`

### 4️⃣ Negative Test (Magic String)

`04_Invalid_Decode_Magic_String_Check.png`

### 5️⃣ Stego Image Validation

`05_Stego_Image_Validation_Output.png`

### 6️⃣ Input vs Output Size Comparison

`06_Input_Output_Image_Size_Comparison.png`

### 7️⃣ Original vs Decoded Text Verification

`07_Original_vs_Decoded_Text_Verification.png`

---

## 🧠 Key Concept

In this project, I used the **LSB (Least Significant Bit)** technique to hide the secret data.
Instead of changing the full pixel value, I modify only the **last bit of each pixel byte**.
Since this changes the pixel intensity only by **±1**, it does not visibly affect the image quality.
This allows the image to look exactly the same while safely carrying the hidden message.

---

## ✅ Sample Verified Output

```text
Original Text : My password is SiddarthS ;)
Decoded Text  : My password is SiddarthS ;)
Status        : MATCHED SUCCESSFULLY
```

---

## 🚀 Conclusion

This project successfully demonstrates **secure text hiding and retrieval using BMP image steganography in C**.
It validates both **image integrity** and **data integrity**, making it a strong foundation project for **file handling, bit manipulation, and embedded-style logic development**.
