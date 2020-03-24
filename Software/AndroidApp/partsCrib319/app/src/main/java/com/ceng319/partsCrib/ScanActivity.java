package com.ceng319.partsCrib;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import io.paperdb.Paper;
import android.widget.Button;
import android.widget.Toast;

import androidx.appcompat.app.AlertDialog;
import androidx.appcompat.app.AppCompatActivity;

import com.ceng319.partsCrib.Prevalent.Prevalent;
import com.r0adkll.slidr.Slidr;
import com.r0adkll.slidr.model.SlidrInterface;

public class ScanActivity extends AppCompatActivity {

    private TextView currentTID;
    private Button cancelBtn;
    private SlidrInterface slidr;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_scan);

        currentTID = (TextView) findViewById(R.id.textView);
        cancelBtn = (Button) findViewById(R.id.button);
        slidr = Slidr.attach(this);

        Paper.init(this);
        String TID = getString(R.string.NoID);
        TID = Paper.book().read(Prevalent.CurrentTID);
        currentTID.setText(TID);

        Intent intent = new Intent(this, myHostApduService.class);
        intent.putExtra("ndefMessage",TID);
        startService(intent);

        cancelBtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                AlertDialog.Builder confirm = new AlertDialog.Builder(ScanActivity.this);
                confirm.setTitle(R.string.remove);
                confirm.setMessage(R.string.are_you_sure);
                confirm.setPositiveButton("Yes", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        Paper.book().write(Prevalent.CurrentTID, getString(R.string.NoID));
                        finish();
                    }
                });
                confirm.setNegativeButton("No", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialogInterface, int i) {
                        Toast.makeText(ScanActivity.this,R.string.cancel,Toast.LENGTH_SHORT).show();
                    }
                });
                AlertDialog dialog = confirm.create();
                dialog.show();
            }
        });

    }
}
