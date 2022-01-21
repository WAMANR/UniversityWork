package com.example.profilemanager.activities;

import android.Manifest;
import android.content.ContentResolver;
import android.content.ContentValues;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.view.View;
import android.widget.Toast;

import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import com.example.profilemanager.databinding.ActivityMainBinding;
import com.example.profilemanager.utilities.Constants;
import com.example.profilemanager.utilities.PreferenceManager;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.android.gms.tasks.Task;
import com.google.firebase.firestore.DocumentReference;
import com.google.firebase.firestore.DocumentSnapshot;
import com.google.firebase.firestore.FirebaseFirestore;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.ListResult;
import com.google.firebase.storage.OnProgressListener;
import com.google.firebase.storage.StorageReference;
import com.google.firebase.storage.UploadTask;
import com.google.zxing.BarcodeFormat;
import com.google.zxing.MultiFormatWriter;
import com.google.zxing.WriterException;
import com.google.zxing.common.BitMatrix;
import com.journeyapps.barcodescanner.BarcodeEncoder;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.Map;
import java.util.Objects;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private ActivityMainBinding binding;
    private PreferenceManager preferenceManager;
    private Bitmap bitmapQr;
    private String otherUserId;
    FirebaseStorage storage = FirebaseStorage.getInstance();
    StorageReference storageReference = storage.getReference();


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        preferenceManager = new PreferenceManager(getApplicationContext());
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        if(!preferenceManager.getBoolean(Constants.KEY_USER_PROFILE)){
            binding.icLogout.setVisibility(View.INVISIBLE);
            binding.icScanner.setVisibility(View.INVISIBLE);
            binding.icDisplayQr.setVisibility(View.INVISIBLE);
            binding.icAdd.setVisibility(View.INVISIBLE);
            binding.icSave.setVisibility(View.INVISIBLE);
            binding.icBack.setVisibility(View.VISIBLE);
            binding.icShare.setVisibility(View.VISIBLE);
            binding.imageText.setVisibility(View.VISIBLE);
        }
        setContentView(binding.getRoot());
        if(preferenceManager.getBoolean(Constants.KEY_USER_PROFILE)) loadProfile(preferenceManager.getString(Constants.KEY_USER_ID));
        else {
            Bundle extras = getIntent().getExtras();
            otherUserId = extras.getString("id");
            countImageStorage();
            loadProfile(otherUserId);
            disableEditText();
        }
        setListeners();
    }

    private void setListeners() {
       binding.icLogout.setOnClickListener(v -> binding.logoutConfirmation.setVisibility(View.VISIBLE));
       binding.logoutNo.setOnClickListener(v -> binding.logoutConfirmation.setVisibility(View.INVISIBLE));
       binding.logoutYes.setOnClickListener(v -> {
           preferenceManager.putBoolean(Constants.KEY_IS_SIGNED_IN, false);
           Intent intent = new Intent(getApplicationContext(), SignInActivity.class);
           startActivity(intent);
           finish();
       });
       binding.icScanner.setOnClickListener(v ->{
           if (ContextCompat.checkSelfPermission(getApplicationContext(), Manifest.permission.CAMERA)
                   == PackageManager.PERMISSION_DENIED){
               ActivityCompat.requestPermissions(this, new String[] {Manifest.permission.CAMERA}, 12);
           }
           else {
               Intent intent = new Intent(getApplicationContext(), ScannerActivity.class);
               startActivity(intent);
           }
               });
       binding.icBack.setOnClickListener(v -> {
           Intent intent = new Intent(getApplicationContext(), MainActivity.class);
           preferenceManager.putBoolean(Constants.KEY_USER_PROFILE, true);
           startActivity(intent);
           finish();
       });
       binding.icSave.setOnClickListener(v ->
               saveProfile());
       binding.icDisplayQr.setOnClickListener(v -> {
           binding.profileScrollView.setVisibility(View.INVISIBLE);
           binding.icLogout.setVisibility(View.INVISIBLE);
           binding.icBack.setVisibility(View.VISIBLE);
           binding.qrCodeDisplay.setVisibility(View.VISIBLE);
           binding.icSave.setVisibility(View.INVISIBLE);
           binding.icSaveQr.setVisibility(View.VISIBLE);
           binding.icDisplayQr.setVisibility(View.INVISIBLE);
           try {
               genQrCode();
           } catch (WriterException e) {
               e.printStackTrace();
           }
       });
       binding.icSaveQr.setOnClickListener(v ->
       {
           try {
               saveImage(bitmapQr);
           } catch (IOException e) {
               e.printStackTrace();
           }
       });
        binding.icAdd.setOnClickListener(v -> {
            Intent intent = new Intent(Intent.ACTION_PICK, MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
            intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
            pickImage.launch(intent);

        });
        binding.icDownload.setOnClickListener(v -> {
            downloadImages();
        });
    }


    private final ActivityResultLauncher<Intent> pickImage = registerForActivityResult(
            new ActivityResultContracts.StartActivityForResult(),
            result -> {
                if(result.getResultCode() == RESULT_OK){
                    if(result.getData() != null){
                        Uri imageUri = result.getData().getData();
                        storeImage(imageUri);
                    }
                }
            }
    );

    public void downloadImages(){

        storageReference.child(otherUserId+"/")
                .listAll()
                .addOnSuccessListener(new OnSuccessListener<ListResult>() {
                    @Override
                    public void onSuccess(ListResult listResult) {
                        for (int i = 0; i < listResult.getItems().size(); i++){
                            StorageReference imageRef = listResult.getItems().get(i);
                            long maxBytes = 1024*1024;
                            imageRef.getBytes(maxBytes).addOnSuccessListener(new OnSuccessListener<byte[]>() {
                                @Override
                                public void onSuccess(byte[] bytes) {
                                    Toast.makeText(getApplicationContext(), "Fine", Toast.LENGTH_SHORT).show();
                                    Bitmap bitmap = BitmapFactory.decodeByteArray(bytes,0, bytes.length);
                                    try {
                                        saveImage(bitmap);
                                    } catch (IOException e) {
                                        e.printStackTrace();
                                    }
                                }
                            });
                        }
                    }
                });
    }

    public void countImageStorage(){
        storageReference.child(otherUserId+"/")
                .listAll()
                .addOnSuccessListener(new OnSuccessListener<ListResult>() {
                    @Override
                    public void onSuccess(ListResult listResult) {
                        for (StorageReference item : listResult.getItems()){
                            Integer count = listResult.getItems().size();
                            if(count >= 1){
                                binding.imageText.setText("There are "+count.toString()+" images to download");
                                binding.icDownload.setVisibility(View.VISIBLE);
                            }
                            else{
                                binding.imageText.setText("There are 0 images to download");
                            }
                        }
                    }
                })
                .addOnFailureListener(new OnFailureListener() {
                    @Override
                    public void onFailure(@NonNull Exception e) {
                        binding.imageText.setText("There is 0 image to download");
                    }
                });
    }

    public void storeImage(Uri imageUri){

        final String randomKey = UUID.randomUUID().toString();
        StorageReference imageRef = storageReference.child(preferenceManager.getString(Constants.KEY_USER_ID) +"/"+ randomKey);
        imageRef.putFile(imageUri)
                .addOnSuccessListener(new OnSuccessListener<UploadTask.TaskSnapshot>() {
                    @Override
                    public void onSuccess(UploadTask.TaskSnapshot taskSnapshot) {
                        Toast.makeText(getApplicationContext(), "Success", Toast.LENGTH_SHORT).show();
                    }
                })
                .addOnFailureListener(new OnFailureListener() {
                    @Override
                    public void onFailure(@NonNull Exception e) {
                        Toast.makeText(getApplicationContext(), "Failure", Toast.LENGTH_SHORT).show();
                    }
                })
                .addOnProgressListener(new OnProgressListener<UploadTask.TaskSnapshot>() {
                    @Override
                    public void onProgress(@NonNull UploadTask.TaskSnapshot snapshot) {
                        Double progressPercent = (100.00 * snapshot.getBytesTransferred() / snapshot.getTotalByteCount());
                        Toast.makeText(getApplicationContext(), progressPercent.toString(), Toast.LENGTH_SHORT).show();
                    }
                });

    }

    @Override
    public void onBackPressed() {
        if(binding.logoutConfirmation.getVisibility() == View.VISIBLE) binding.logoutConfirmation.setVisibility(View.INVISIBLE);
        else if(preferenceManager.getBoolean(Constants.KEY_USER_PROFILE) && binding.icBack.getVisibility() == View.INVISIBLE) binding.logoutConfirmation.setVisibility(View.VISIBLE);
        else {
            Intent intent = new Intent(getApplicationContext(), MainActivity.class);
            preferenceManager.putBoolean(Constants.KEY_USER_PROFILE, true);
            startActivity(intent);
            finish();
        }
    }

    private void saveImage(Bitmap bitmap) throws IOException {

        OutputStream fos;
        final String randomKey = UUID.randomUUID().toString();

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.Q) {
            ContentResolver resolver = getApplicationContext().getContentResolver();
            ContentValues contentValues = new ContentValues();
            contentValues.put(MediaStore.MediaColumns.DISPLAY_NAME, "profile");
            contentValues.put(MediaStore.MediaColumns.MIME_TYPE, "image/png");
            contentValues.put(MediaStore.MediaColumns.RELATIVE_PATH, "DCIM/" + preferenceManager.getString(Constants.KEY_USER_ID));
            Uri imageUri = resolver.insert(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, contentValues);
            fos = resolver.openOutputStream(imageUri);
        } else {
            String imagesDir = Environment.getExternalStoragePublicDirectory(
                    Environment.DIRECTORY_DCIM).toString() + File.separator + "Profile Manager";

            File file = new File(imagesDir);

            if (!file.exists()) {
                file.mkdir();
            }

            File image = new File(imagesDir, preferenceManager.getString(Constants.KEY_USER_ID) + randomKey+ ".png");
            fos = new FileOutputStream(image);

        }
        bitmap.compress(Bitmap.CompressFormat.PNG, 100, fos);
        Toast.makeText(getApplicationContext(), "File saved successfully", Toast.LENGTH_SHORT).show();
        fos.flush();
        fos.close();
    }

    private void loadProfile(String id){

        FirebaseFirestore database = FirebaseFirestore.getInstance();
        DocumentReference documentReference = database.collection(Constants.KEY_COLLECTION_USERS).document(id);
        documentReference.get().addOnCompleteListener(new OnCompleteListener<DocumentSnapshot>() {
            @Override
            public void onComplete(@NonNull Task<DocumentSnapshot> task) {
                if(task.isSuccessful()){
                    DocumentSnapshot documentSnapshot = task.getResult();
                    assert documentSnapshot != null;
                    if(documentSnapshot.exists()){
                        //exist
                        Map documentData = documentSnapshot.getData();
                        assert documentData != null;
                        enterUserData(documentData);
                    }
                    //don't exist
                }
                //task unsuccessful
            }
        });
    }

    private void enterUserData(Map documentData){

        if(documentData.get("firstname") != null) binding.editFirstname.setText(Objects.requireNonNull(documentData.get("firstname")).toString());
        if(documentData.get("lastname") != null) binding.editLastname.setText(documentData.get("lastname").toString());
        if(documentData.get("usename") != null) binding.editUsename.setText(documentData.get("usename").toString());
        if(documentData.get("birthDate") != null) binding.editBirthDate.setText(documentData.get("birthDate").toString());
        if(documentData.get("textNationalityOther") != null) binding.editNationalityOther.setText(documentData.get("textNationalityOther").toString());
        if(documentData.get("birthCity") != null) binding.editBirthCity.setText(documentData.get("birthCity").toString());
        if(documentData.get("birthCounty") != null) binding.editBirthCounty.setText(documentData.get("birthCounty").toString());
        if(documentData.get("birthOther") != null) binding.editBirthOther.setText(documentData.get("birthOther").toString());
        if(documentData.get("fullAdress") != null) binding.editFullAdress.setText(documentData.get("fullAdress").toString());
        if(documentData.get("postalCode") != null) binding.editPostalCode.setText(documentData.get("postalCode").toString());
        if(documentData.get("city") != null) binding.editCity.setText(documentData.get("city").toString());
        if(documentData.get("country") != null) binding.editCountry.setText(documentData.get("country").toString());
        if(documentData.get("phoneNumber") != null) binding.editPhoneNumber.setText(documentData.get("phoneNumber").toString());
        if(documentData.get("mail") != null) binding.editMailAdress.setText(documentData.get("mail").toString());

        if(documentData.get("supervisory") != null) {
            if (documentData.get("supervisory").equals(true))
                binding.checkboxSupervisory.setChecked(true);
            else binding.checkBoxSupervised.setChecked(true);
        }

        if(documentData.get("nationalityFrench") != null) {
            if (documentData.get("nationalityFrench").equals(true))
                binding.checkboxNationalityFrench.setChecked(true);
            else binding.checkboxNationalityOther.setChecked(true);
        }

        if(documentData.get("birthInFrance") != null) {
            if (documentData.get("birthInFrance").equals(true))
                binding.checkboxBirthFrance.setChecked(true);
            else binding.checkboxBirthOther.setChecked(true);
        }

        if(documentData.get("gender") != null) {
            if (documentData.get("gender").equals("male")) binding.checkboxMale.setChecked(true);
            else if (documentData.get("gender").equals("female"))
                binding.checkboxFemale.setChecked(true);
            else binding.checkBoxOther.setChecked(true);
        }
    }

    private void saveProfile(){

        FirebaseFirestore database = FirebaseFirestore.getInstance();
        DocumentReference documentReference = database.collection(Constants.KEY_COLLECTION_USERS).document(preferenceManager.getString(Constants.KEY_USER_ID));
        documentReference.update(
                "firstname", binding.editFirstname.getText().toString(),
                "lastname", binding.editLastname.getText().toString(),
                "usename", binding.editUsename.getText().toString(),
                "birthDate", binding.editBirthDate.getText().toString(),
                "birthCity", binding.editBirthCity.getText().toString(),
                "birthCounty", binding.editBirthCounty.getText().toString(),
                "birthOther", binding.editBirthOther.getText().toString(),
                "textNationalityOther", binding.editNationalityOther.getText().toString(),
                "fullAdress", binding.editFullAdress.getText().toString(),
                "postalCode", binding.editPostalCode.getText().toString(),
                "city", binding.editCity.getText().toString(),
                "country", binding.editCountry.getText().toString(),
                "phoneNumber", binding.editPhoneNumber.getText().toString(),
                "mail", binding.editMailAdress.getText().toString()
        );




        if(binding.checkboxSupervisory.isChecked()) documentReference.update("supervisory", true);
        else documentReference.update("supervisory", false);

        if(binding.checkboxNationalityFrench.isChecked()) documentReference.update("nationalityFrench", true);
        else documentReference.update("nationalityFrench", false);

        if(binding.checkboxBirthFrance.isChecked()) documentReference.update("birthInFrance", true);
        else documentReference.update("birthInFrance", false);

        if(binding.checkboxMale.isChecked()) documentReference.update("gender", "male");
        else if(binding.checkboxFemale.isChecked()) documentReference.update("gender", "female");
        else if(binding.checkBoxOther.isChecked()) documentReference.update("gender", "other");
        Toast.makeText(getApplicationContext(), "Profile Update successful", Toast.LENGTH_SHORT).show();
    }

    private void genQrCode() throws WriterException {
        MultiFormatWriter writer = new MultiFormatWriter();
        BitMatrix matrix = writer.encode(preferenceManager.getString(Constants.KEY_USER_ID), BarcodeFormat.QR_CODE
        , 350, 350);
        BarcodeEncoder encoder = new BarcodeEncoder();
        bitmapQr = encoder.createBitmap(matrix);
        binding.qrCodeDisplay.setImageBitmap(bitmapQr);

    }


    private void disableEditText(){

        //editText
        binding.editFirstname.setFocusable(false);
        binding.editLastname.setFocusable(false);
        binding.editUsename.setFocusable(false);
        binding.editBirthDate.setFocusable(false);
        binding.editNationalityOther.setFocusable(false);
        binding.editBirthCity.setFocusable(false);
        binding.editBirthCounty.setFocusable(false);
        binding.editBirthOther.setFocusable(false);
        binding.editFullAdress.setFocusable(false);
        binding.editPostalCode.setFocusable(false);
        binding.editCity.setFocusable(false);
        binding.editCountry.setFocusable(false);
        binding.editPhoneNumber.setFocusable(false);
        binding.editMailAdress.setFocusable(false);
        //Checkbox
        binding.checkboxSupervisory.setEnabled(false);
        binding.checkBoxSupervised.setEnabled(false);
        binding.checkboxMale.setEnabled(false);
        binding.checkboxFemale.setEnabled(false);
        binding.checkBoxOther.setEnabled(false);
        binding.checkboxNationalityOther.setEnabled(false);
        binding.checkboxNationalityFrench.setEnabled(false);
        binding.checkboxBirthFrance.setEnabled(false);
        binding.checkboxBirthOther.setEnabled(false);
    }

}