
package com.SmartRelay_v0_1;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;

public class SmartRelay_v0_1 extends Activity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        /* Create a TextView and set its text to "Hello world" */
        TextView  tv = new TextView(this);
        tv.setText("This is an app for displaying data usage from AC units");
        setContentView(tv);
    }
}
