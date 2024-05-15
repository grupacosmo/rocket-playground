package pl.edu.pk.cosmo.rakieta.service;

import com.google.auth.oauth2.GoogleCredentials;
import com.google.firebase.FirebaseApp;
import com.google.firebase.FirebaseOptions;
import com.google.firebase.database.FirebaseDatabase;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

public class FireBaseService {

    FirebaseDatabase db;

    public FireBaseService() throws IOException {

        FileInputStream serviceAccount =
                new FileInputStream(getClass().getClassLoader().getResource("cosmopklora-firebase-adminsdk-gnt9r-ffb382aeb8.json").getFile());
        FirebaseOptions options = new FirebaseOptions.Builder()
                .setCredentials(GoogleCredentials.fromStream(serviceAccount))
                .setDatabaseUrl("https://cosmopklora-default-rtdb.europe-west1.firebasedatabase.app/")
                .build();

        FirebaseApp.initializeApp(options);

        db = FirebaseDatabase.getInstance();
    }

    public FirebaseDatabase getDb() {
        return db;
    }

}
