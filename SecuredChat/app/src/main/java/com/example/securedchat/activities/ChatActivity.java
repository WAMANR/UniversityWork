package com.example.securedchat.activities;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Build;
import android.os.Bundle;
import android.util.Base64;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.RequiresApi;

import com.example.securedchat.adapters.ChatAdapter;
import com.example.securedchat.databinding.ActivityChatBinding;
import com.example.securedchat.models.ChatMessage;
import com.example.securedchat.models.User;
import com.example.securedchat.utilities.Constants;
import com.example.securedchat.utilities.PreferenceManager;
import com.example.securedchat.utilities.SecurityModule;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.firestore.DocumentChange;
import com.google.firebase.firestore.DocumentReference;
import com.google.firebase.firestore.DocumentSnapshot;
import com.google.firebase.firestore.EventListener;
import com.google.firebase.firestore.FirebaseFirestore;
import com.google.firebase.firestore.QuerySnapshot;

import java.io.UnsupportedEncodingException;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.spec.InvalidKeySpecException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Locale;
import java.util.Objects;

import javax.crypto.BadPaddingException;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;

@RequiresApi(api = Build.VERSION_CODES.O)
public class ChatActivity extends BaseActivity {

    private ActivityChatBinding binding;
    private User receiverUser;
    private List<ChatMessage> chatMessages;
    private ChatAdapter chatAdapter;
    private PreferenceManager preferenceManager;
    private FirebaseFirestore database;
    private String conversionId = null;
    private Boolean isReceiverAvailable = false;


    private Boolean isConversionExist = false;
    private Boolean sender;
    private Integer chatSecurity;
    private String security;
    private String iv;
    private static final String LOCAL_STORAGE = "MY_STORAGE_FILE";


    @RequiresApi(api = Build.VERSION_CODES.O)
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        binding = ActivityChatBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());
        setListerners();
        loadReceiverDetails();
        init();
        listenMessages();
    }


    private void init(){
        preferenceManager = new PreferenceManager(getApplicationContext());
        chatMessages = new ArrayList<>();
        chatAdapter = new ChatAdapter(
                chatMessages,
                getBitmapFromEncodedString(receiverUser.image),
                preferenceManager.getString(Constants.KEY_USER_ID)
        );
        binding.chatRecyclerView.setAdapter(chatAdapter);
        database = FirebaseFirestore.getInstance();
        conversionExistSender();

    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    private void sendInvitation() throws NoSuchAlgorithmException {
        SecurityModule securityModule = new SecurityModule();
        securityModule.RSA_generateKeyPair();
        String publicKey = securityModule.RSA_getKeyString(true);
        String privateKey = securityModule.RSA_getKeyString(false);
        storeLocal("privateKey", privateKey);
        HashMap<String, Object> conversion = new HashMap<>();
        conversion.put(Constants.KEY_SENDER_ID, preferenceManager.getString(Constants.KEY_USER_ID));
        conversion.put(Constants.KEY_SENDER_NAME, preferenceManager.getString(Constants.KEY_NAME));
        conversion.put(Constants.KEY_SENDER_IMAGE, preferenceManager.getString(Constants.KEY_IMAGE));
        conversion.put(Constants.KEY_RECEIVER_ID, receiverUser.id);
        conversion.put(Constants.KEY_RECEIVER_NAME, receiverUser.name);
        conversion.put(Constants.KEY_RECEIVER_IMAGE, receiverUser.image);
        conversion.put(Constants.KEY_LAST_MESSAGE, "");
        conversion.put(Constants.KEY_TIMESTAMP, new Date());
        conversion.put(Constants.KEY_CHAT_SECURITY, 0);
        conversion.put(Constants.KEY_SECURITY, publicKey);
        conversion.put(Constants.KEY_IV, "");
        addConversion(conversion);
        Toast.makeText(getApplicationContext(), "Invitation sent", Toast.LENGTH_SHORT).show();
        binding.inviteFriend.setVisibility(View.INVISIBLE);
        binding.awaitingAcceptance.setVisibility(View.VISIBLE);
    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    private void acceptInvitation() throws InvalidKeySpecException, NoSuchAlgorithmException, IllegalBlockSizeException, InvalidKeyException, BadPaddingException, NoSuchPaddingException, InvalidAlgorithmParameterException, UnsupportedEncodingException {

        SecurityModule securityModule = new SecurityModule();
        securityModule.AES_generateKey();
        securityModule.RSA_putKeyString(security, true);
        String secretKey = securityModule.AES_getSecretKey();
        String IV = securityModule.AES_getIV();
        storeLocal("secretKey", secretKey);
        storeLocal("iv",IV);
        String secretKeyEncrypted = securityModule.RSA_encrypt(secretKey);
        String ivEncrypted = securityModule.RSA_encrypt(IV);

        DocumentReference documentReference =
                database.collection(Constants.KEY_COLLECTION_CONVERSATIONS).document(conversionId);
        documentReference.update(
                Constants.KEY_CHAT_SECURITY, 1,
                Constants.KEY_SECURITY, secretKeyEncrypted,
                Constants.KEY_IV, ivEncrypted,
                Constants.KEY_TIMESTAMP, new Date()
        );
        Toast.makeText(getApplicationContext(), "Invitation accepted", Toast.LENGTH_SHORT).show();
        binding.acceptFriend.setVisibility(View.INVISIBLE);
        binding.inputMessage.setVisibility(View.VISIBLE);
        binding.layoutSend.setVisibility(View.VISIBLE);

    }

    private void storeLocal(String key, String value){
        getSharedPreferences(LOCAL_STORAGE, MODE_PRIVATE)
                .edit()
                .putString(key, value)
                .apply();
    }

    private String getLocal(String key){
        return getSharedPreferences(LOCAL_STORAGE, MODE_PRIVATE).getString(key, "");
    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    private void senderGetAes() throws InvalidKeySpecException, NoSuchAlgorithmException, IllegalBlockSizeException, InvalidKeyException, BadPaddingException, NoSuchPaddingException, UnsupportedEncodingException {
        SecurityModule securityModule = new SecurityModule();
        String privateKey = getLocal("privateKey");
        securityModule.RSA_putKeyString(privateKey, false);
        String secretKey = securityModule.RSA_decrypt(security);
        String IV = securityModule.RSA_decrypt(iv);
        storeLocal("secretKey", secretKey);
        storeLocal("iv", IV);

    }




    @RequiresApi(api = Build.VERSION_CODES.O)
    private void changeDisplay() throws NoSuchPaddingException, BadPaddingException, NoSuchAlgorithmException, IllegalBlockSizeException, UnsupportedEncodingException, InvalidKeyException, InvalidKeySpecException, InvalidAlgorithmParameterException {

        if(!isConversionExist){
            binding.inputMessage.setVisibility(View.INVISIBLE);
            binding.layoutSend.setVisibility(View.INVISIBLE);
            binding.inviteFriend.setVisibility(View.VISIBLE);
        }

        else{
            if(chatSecurity == 0){
                if (sender){
                    binding.inputMessage.setVisibility(View.INVISIBLE);
                    binding.layoutSend.setVisibility(View.INVISIBLE);
                    binding.inviteFriend.setVisibility(View.INVISIBLE);
                    binding.awaitingAcceptance.setVisibility(View.VISIBLE);
                }
                else{
                    binding.inputMessage.setVisibility(View.INVISIBLE);
                    binding.layoutSend.setVisibility(View.INVISIBLE);
                    binding.acceptFriend.setVisibility(View.VISIBLE);
                }
            }
            else{
                if(sender && chatSecurity == 1){
                    senderGetAes();
                    binding.awaitingAcceptance.setVisibility(View.INVISIBLE);
                }
            }
        }
    }

    private void conversionExistSender(){
        database.collection(Constants.KEY_COLLECTION_CONVERSATIONS)
                .whereEqualTo(Constants.KEY_SENDER_ID, preferenceManager.getString(Constants.KEY_USER_ID))
                .whereEqualTo(Constants.KEY_RECEIVER_ID, receiverUser.id)
                .get()
                .addOnCompleteListener(new OnCompleteListener<QuerySnapshot>() {
                    @RequiresApi(api = Build.VERSION_CODES.O)
                    @Override
                    public void onComplete(@NonNull Task<QuerySnapshot> task) {
                        if (task.isSuccessful() && task.getResult() != null && task.getResult().getDocuments().size() > 0){
                            isConversionExist = true;
                            sender = true;
                            conversionId =task.getResult().getDocuments().get(0).getId();
                            chatSecurity = Integer.parseInt(task.getResult().getDocuments().get(0).get(Constants.KEY_CHAT_SECURITY).toString());
                            if (chatSecurity == 1){
                                security = task.getResult().getDocuments().get(0).get(Constants.KEY_SECURITY).toString();
                                iv = task.getResult().getDocuments().get(0).get(Constants.KEY_IV).toString();
                            }
                            try {
                                changeDisplay();
                            } catch (NoSuchPaddingException | BadPaddingException | NoSuchAlgorithmException | IllegalBlockSizeException | UnsupportedEncodingException | InvalidKeyException | InvalidKeySpecException | InvalidAlgorithmParameterException e) {
                                e.printStackTrace();
                            }
                        }
                        else{
                            conversionExistReceiver();
                        }
                    }
                });
    }

    private void conversionExistReceiver(){
        database.collection(Constants.KEY_COLLECTION_CONVERSATIONS)
                .whereEqualTo(Constants.KEY_SENDER_ID, receiverUser.id)
                .whereEqualTo(Constants.KEY_RECEIVER_ID, preferenceManager.getString(Constants.KEY_USER_ID))
                .get()
                .addOnCompleteListener(new OnCompleteListener<QuerySnapshot>() {
                    @RequiresApi(api = Build.VERSION_CODES.O)
                    @Override
                    public void onComplete(@NonNull Task<QuerySnapshot> task) {
                        if (task.isSuccessful() && task.getResult() != null && task.getResult().getDocuments().size() > 0){
                            isConversionExist = true;
                            sender = false;
                            conversionId = task.getResult().getDocuments().get(0).getId();
                            chatSecurity = Integer.parseInt(task.getResult().getDocuments().get(0).get(Constants.KEY_CHAT_SECURITY).toString());
                            if(chatSecurity == 0){
                                security = task.getResult().getDocuments().get(0).get(Constants.KEY_SECURITY).toString();
                            }
                        }
                        try {
                            changeDisplay();
                        } catch (NoSuchPaddingException | BadPaddingException | NoSuchAlgorithmException | IllegalBlockSizeException | UnsupportedEncodingException | InvalidKeyException | InvalidKeySpecException | InvalidAlgorithmParameterException e) {
                            e.printStackTrace();
                        }
                    }
                });
    }

    private void sendMessage() throws NoSuchPaddingException, InvalidAlgorithmParameterException, NoSuchAlgorithmException, IllegalBlockSizeException, BadPaddingException, InvalidKeyException {
        HashMap<String, Object> message = new HashMap<>();
        message.put(Constants.KEY_SENDER_ID, preferenceManager.getString(Constants.KEY_USER_ID));
        message.put(Constants.KEY_RECEIVER_ID, receiverUser.id);
        String messageVanilla = binding.inputMessage.getText().toString();
        SecurityModule securityModule = new SecurityModule();
        String secretKey = getLocal("secretKey");
        String IV = getLocal("iv");
        securityModule.AES_putSecretKeyAndIV(secretKey, IV);
        message.put(Constants.KEY_MESSAGE, securityModule.AES_encrypt(messageVanilla));
        message.put(Constants.KEY_TIMESTAMP, new Date());
        database.collection(Constants.KEY_COLLECTION_CHAT).add(message);
        updateConversion(binding.inputMessage.getText().toString());
        binding.inputMessage.setText(null);
    }

    private void listenAvailabilityOfReceiver(){
       database.collection(Constants.KEY_COLLECTION_USERS).document(
               receiverUser.id
       ).addSnapshotListener(ChatActivity.this, (value, error) -> {
           if(error != null){
               return;
           }
           if(value != null){
               if(value.getLong(Constants.KEY_AVAILABILITY) != null){
                   int availability = Objects.requireNonNull(
                           value.getLong(Constants.KEY_AVAILABILITY)
                   ).intValue();
                   isReceiverAvailable = availability == 1;
               }
           }
           if(isReceiverAvailable){
               binding.textAvailability.setVisibility(View.VISIBLE);
           }
           else{
               binding.textAvailability.setVisibility(View.GONE);
           }
       });
    }

    private void listenMessages(){
        database.collection(Constants.KEY_COLLECTION_CHAT)
                .whereEqualTo(Constants.KEY_SENDER_ID, preferenceManager.getString(Constants.KEY_USER_ID))
                .whereEqualTo(Constants.KEY_RECEIVER_ID, receiverUser.id)
                .addSnapshotListener(eventListener);
        database.collection(Constants.KEY_COLLECTION_CHAT)
                .whereEqualTo(Constants.KEY_SENDER_ID, receiverUser.id)
                .whereEqualTo(Constants.KEY_RECEIVER_ID, preferenceManager.getString(Constants.KEY_USER_ID))
                .addSnapshotListener(eventListener);
    }

    private final EventListener<QuerySnapshot> eventListener = (value, error) -> {
        if(error != null){
            return;
        }
        if(value != null && !value.isEmpty()){
            SecurityModule securityModule = new SecurityModule();
            String secretKey = getLocal("secretKey");
            String IV = getLocal("iv");
            securityModule.AES_putSecretKeyAndIV(secretKey, IV);
            int count = chatMessages.size();
            for (DocumentChange documentChange : value.getDocumentChanges()){
                if(documentChange.getType() == DocumentChange.Type.ADDED){
                    ChatMessage chatMessage = new ChatMessage();
                    chatMessage.senderId = documentChange.getDocument().getString(Constants.KEY_SENDER_ID);
                    chatMessage.receiverId = documentChange.getDocument().getString(Constants.KEY_RECEIVER_ID);
                    String encrypted = documentChange.getDocument().getString(Constants.KEY_MESSAGE);
                    try {
                        chatMessage.message = securityModule.AES_decrypt(encrypted);
                    } catch (NoSuchPaddingException | NoSuchAlgorithmException | BadPaddingException | IllegalBlockSizeException | InvalidAlgorithmParameterException | InvalidKeyException e) {
                        e.printStackTrace();
                    }
                    chatMessage.dateTime = getReadableDateTime(documentChange.getDocument().getDate(Constants.KEY_TIMESTAMP));
                    chatMessage.dateObject = documentChange.getDocument().getDate(Constants.KEY_TIMESTAMP);
                    chatMessages.add(chatMessage);
                }
            }
            Collections.sort(chatMessages, (obj1, obj2) -> obj1.dateObject.compareTo(obj2.dateObject));
            if(count == 0){
                chatAdapter.notifyDataSetChanged();
            }
            else{
                chatAdapter.notifyItemRangeInserted(chatMessages.size(), chatMessages.size());
                binding.chatRecyclerView.smoothScrollToPosition(chatMessages.size()-1);
            }
            binding.chatRecyclerView.setVisibility(View.VISIBLE);
        }
        binding.progressBar.setVisibility(View.GONE);
        if(conversionId == null){
            checkForConversion();
        }
    };

    private Bitmap getBitmapFromEncodedString(String encodedImage){
        byte[] bytes = Base64.decode(encodedImage, Base64.DEFAULT);
        return BitmapFactory.decodeByteArray(bytes, 0, bytes.length);

    }

    private void loadReceiverDetails(){
        receiverUser = (User) getIntent().getSerializableExtra(Constants.KEY_USER);
        binding.textName.setText(receiverUser.name);
    }

    @RequiresApi(api = Build.VERSION_CODES.O)
    private void setListerners(){
        binding.imageBack.setOnClickListener(v -> onBackPressed());
        binding.layoutSend.setOnClickListener(v -> {
            try {
                sendMessage();
            } catch (NoSuchPaddingException | InvalidAlgorithmParameterException | NoSuchAlgorithmException | IllegalBlockSizeException | BadPaddingException | InvalidKeyException e) {
                e.printStackTrace();
            }
        });
        binding.inviteFriend.setOnClickListener(v -> {
            try {
                sendInvitation();
            } catch (NoSuchAlgorithmException e) {
                e.printStackTrace();
            }
        });
        binding.acceptFriend.setOnClickListener(v -> {
            try {
                acceptInvitation();
            } catch (InvalidKeySpecException | NoSuchAlgorithmException | IllegalBlockSizeException | InvalidKeyException | BadPaddingException | NoSuchPaddingException | InvalidAlgorithmParameterException | UnsupportedEncodingException e) {
                e.printStackTrace();
            }
        });
    }

    private String getReadableDateTime(Date date){
        return new SimpleDateFormat("dd MMMM, yyyy - hh:mm a", Locale.getDefault()).format(date);
    }

    private void addConversion(HashMap<String, Object> conversion){
        database.collection(Constants.KEY_COLLECTION_CONVERSATIONS)
                .add(conversion)
                .addOnSuccessListener(documentReference -> conversionId = documentReference.getId());
    }

    private void updateConversion(String message){
        DocumentReference documentReference =
                database.collection(Constants.KEY_COLLECTION_CONVERSATIONS).document(conversionId);
        documentReference.update(
            Constants.KEY_LAST_MESSAGE, message,
            Constants.KEY_TIMESTAMP, new Date()
        );
    }

    private void checkForConversion(){
        if(chatMessages.size() != 0){
            checkForConversionRemotely(
                    preferenceManager.getString(Constants.KEY_USER_ID),
                    receiverUser.id
            );
            checkForConversionRemotely(
                    receiverUser.id,
                    preferenceManager.getString(Constants.KEY_USER_ID)
            );
        }
    }

    private void checkForConversionRemotely(String senderId, String receiverId){
        database.collection(Constants.KEY_COLLECTION_CONVERSATIONS)
                .whereEqualTo(Constants.KEY_SENDER_ID, senderId)
                .whereEqualTo(Constants.KEY_RECEIVER_ID, receiverId)
                .get()
                .addOnCompleteListener(conversionOnCompleteListener);
    }

    private final OnCompleteListener<QuerySnapshot> conversionOnCompleteListener = task -> {
        if(task.isSuccessful() && task.getResult() != null && task.getResult().getDocuments().size() > 0){
            DocumentSnapshot documentSnapshot = task.getResult().getDocuments().get(0);
            conversionId = documentSnapshot.getId();
        }
    };

    @Override
    protected void onResume() {
        super.onResume();
        listenAvailabilityOfReceiver();
    }
}