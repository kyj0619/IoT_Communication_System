package com.android.empty_activity;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {
    private TextView textView;
    private TextView textView2;
    private TextView textView3;
//    private EditText editText;
//    private Button button;

    DatabaseReference mRootRef = FirebaseDatabase.getInstance().getReference();
    DatabaseReference conditionRef = mRootRef.child("Data");
    DatabaseReference test = mRootRef.child("test/string/");
    DatabaseReference d_number = mRootRef.child("test/double/");



    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        textView = (TextView) findViewById(R.id.textView);
        textView2 = (TextView) findViewById(R.id.textView2);
        textView3 = (TextView) findViewById(R.id.textView3);
//        editText = (EditText) findViewById(R.id.editText);
//        button = (Button) findViewById(R.id.button);
    }

    @Override
    protected void onStart() {
        super.onStart();

        conditionRef.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                int text = dataSnapshot.getValue(int.class);
                textView.setText(String.valueOf(text));
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {

            }
        });
        test.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                String number = dataSnapshot.getValue(String.class);
                textView2.setText(number);
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {

            }
        });

        d_number.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                double double_number = dataSnapshot.getValue(double.class);
                textView3.setText(String.valueOf(double_number));
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {

            }
        });

//        button.setOnClickListener(new View.OnClickListener() {
//            @Override
//            public void onClick(View v) {
//                conditionRef.setValue(editText.getText().toString());
//            }
//        });
    }
}