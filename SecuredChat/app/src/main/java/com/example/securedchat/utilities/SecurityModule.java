package com.example.securedchat.utilities;

import android.os.Build;
import android.util.Log;
import android.widget.Toast;

import androidx.annotation.RequiresApi;

import java.io.UnsupportedEncodingException;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.Key;
import java.security.KeyFactory;
import java.security.KeyPair;
import java.security.KeyPairGenerator;
import java.security.NoSuchAlgorithmException;
import java.security.PrivateKey;
import java.security.PublicKey;
import java.security.SecureRandom;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.PKCS8EncodedKeySpec;
import java.security.spec.X509EncodedKeySpec;
import java.util.Base64;
import java.util.Random;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.KeyGenerator;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.spec.GCMParameterSpec;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;

public class SecurityModule {

    public PublicKey publicKey; //RSA
    public PrivateKey privateKey; //RSA

    public SecretKey secretKey; //AES
    public byte[] IV;

    public SecurityModule() {
    }

    public void AES_generateKey() throws NoSuchAlgorithmException {
        KeyGenerator keyGenerator = KeyGenerator.getInstance("AES");
        keyGenerator.init(128);
        byte[] iv = new byte[12];
        new Random().nextBytes(iv);
        IV = iv;
        secretKey = keyGenerator.generateKey();
    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    public String AES_encrypt(String message) throws NoSuchPaddingException, NoSuchAlgorithmException, InvalidKeyException, BadPaddingException, IllegalBlockSizeException, InvalidAlgorithmParameterException {
        byte[] messageToBytes = message.getBytes();
        Cipher encryptionCipher = Cipher.getInstance("AES/GCM/NoPadding");
        encryptionCipher.init(Cipher.ENCRYPT_MODE, secretKey, new IvParameterSpec(IV));
        byte[] encryptedBytes = encryptionCipher.doFinal(messageToBytes);
        return encode(encryptedBytes);
    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    public String AES_decrypt(String encryptedMessage) throws NoSuchPaddingException, NoSuchAlgorithmException, BadPaddingException, IllegalBlockSizeException, InvalidAlgorithmParameterException, InvalidKeyException {
        byte[] messageToBytes = decode(encryptedMessage);
        Cipher cipher = Cipher.getInstance("AES/GCM/NoPadding");
        GCMParameterSpec spec = new GCMParameterSpec(128, IV);
        cipher.init(Cipher.DECRYPT_MODE, secretKey, spec);
        byte[] decryptedBytes = cipher.doFinal(messageToBytes);
        return new String(decryptedBytes);

    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    public String AES_getSecretKey(){
        byte[] keyReturn = secretKey.getEncoded();
        return encode(keyReturn);
    }
    @RequiresApi(api = Build.VERSION_CODES.O)
    public String AES_getIV(){
        return encode(IV);
    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    public void AES_putSecretKeyAndIV(String keyString, String IvString) {
        byte[] returnKey = decode(keyString);
        secretKey = new SecretKeySpec(returnKey, 0, returnKey.length, "AES");
        IV = decode(IvString);
    }

    public void RSA_generateKeyPair() throws NoSuchAlgorithmException {
        KeyPairGenerator keyPairGenerator = KeyPairGenerator.getInstance("RSA");
        keyPairGenerator.initialize(2048);
        KeyPair keyPair = keyPairGenerator.genKeyPair();
        publicKey = keyPair.getPublic();
        privateKey = keyPair.getPrivate();
    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    public String RSA_encrypt(String message) throws NoSuchPaddingException, NoSuchAlgorithmException, InvalidKeyException, BadPaddingException, IllegalBlockSizeException {
        byte [] messageToByte = message.getBytes();
        Cipher cipher = Cipher.getInstance("RSA/ECB/PKCS1Padding");
        cipher.init(Cipher.ENCRYPT_MODE, publicKey);
        byte[] encryptedBytes = cipher.doFinal(messageToByte);
        return encode(encryptedBytes);
    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    public String RSA_decrypt(String encryptedMessage) throws NoSuchPaddingException, NoSuchAlgorithmException, InvalidKeyException, UnsupportedEncodingException, BadPaddingException, IllegalBlockSizeException {
        byte [] encryptedBytes = decode(encryptedMessage);
        Cipher cipher = Cipher.getInstance("RSA/ECB/PKCS1Padding");
        cipher.init(Cipher.DECRYPT_MODE, privateKey);
        byte[] decryptedMessage = cipher.doFinal(encryptedBytes);
        return new String(decryptedMessage, "UTF8");
    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    private String encode(byte[] data){
        return Base64.getEncoder().encodeToString(data);
    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    private byte[] decode(String data){
        return Base64.getDecoder().decode(data);
    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    public String RSA_getKeyString(Boolean pub){
        byte [] keyReturn;
        if(pub){
            keyReturn = publicKey.getEncoded();
        }
        else{
            keyReturn = privateKey.getEncoded();
        }
        return encode(keyReturn);
    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    public void RSA_putKeyString(String keyString, Boolean pub) throws NoSuchAlgorithmException, InvalidKeySpecException {
        byte[] returnKey = decode(keyString);
        KeyFactory keyFactory = KeyFactory.getInstance("RSA");
        if(pub){
            publicKey = (PublicKey) keyFactory.generatePublic(new X509EncodedKeySpec(returnKey));
        }
        else{
            privateKey = (PrivateKey) keyFactory.generatePrivate(new PKCS8EncodedKeySpec(returnKey));
        }

    }


}
