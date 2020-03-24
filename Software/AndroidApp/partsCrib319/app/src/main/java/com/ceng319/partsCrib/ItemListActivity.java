package com.ceng319.partsCrib;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.view.MenuItemCompat;

import android.content.Intent;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
//import android.widget.SearchView;
import android.widget.Toast;
import androidx.appcompat.widget.SearchView;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;
import com.r0adkll.slidr.Slidr;
import com.r0adkll.slidr.model.SlidrInterface;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;

public class ItemListActivity extends AppCompatActivity{
    private RequestQueue mQueue;
    private ArrayList<String> itemList = new ArrayList<>();
    private ArrayList<String> sidList = new ArrayList<>();
    private ListView mListItems;
    private ArrayAdapter arrayAdapter;
    private String selectedCat;
    private SlidrInterface slidr;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_item_list);
        Intent intent = getIntent();
        getSupportActionBar().setTitle(R.string.item_name);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        selectedCat = intent.getStringExtra("Category");
        mListItems = (ListView)findViewById(R.id.listViewItems);
        slidr = Slidr.attach(this);
        mQueue = Volley.newRequestQueue(this);

        jsonParse();
        arrayAdapter = new ArrayAdapter(this,android.R.layout.simple_list_item_1);

        mListItems.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                String selectedItem = sidList.get(i);
                Intent newIntent = new Intent(ItemListActivity.this,ItemActivity.class);
                newIntent.putExtra("Item",selectedItem);
                ItemListActivity.this.startActivity(newIntent);
            }
        });

        Toast.makeText(ItemListActivity.this,"Going to: "+ selectedCat,Toast.LENGTH_SHORT).show();
    }

    private void jsonParse(){

        String url = "http://apollo.humber.ca/~n01156096/CENG319/query.php?category="+selectedCat;

        JsonObjectRequest request =  new JsonObjectRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        try {
                            JSONArray jsonArray = response.getJSONArray("Items");
                            for (int i = 0; i< jsonArray.length(); i++){
                                //Append to array list to create list view
                                itemList.add(String.valueOf(jsonArray.get(i)));

                                mListItems.setAdapter(arrayAdapter);
                                arrayAdapter.clear();
                                arrayAdapter.addAll(itemList);
                                arrayAdapter.notifyDataSetChanged();


                            }

                            JSONArray jsonArray2 = response.getJSONArray("SID");
                            for (int i = 0; i< jsonArray.length(); i++){
                                sidList.add(String.valueOf(jsonArray2.get(i)));
                            }
                        } catch (JSONException e) {
                            e.printStackTrace();
                        }
                    }
                }, new Response.ErrorListener() {
            @Override
            public void onErrorResponse(VolleyError error) {
                error.printStackTrace();
            }
        });
        mQueue.add(request);

    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu, menu);

        MenuItem searchItem = menu.findItem(R.id.item_search);
        SearchView searchView = (SearchView) MenuItemCompat.getActionView(searchItem);

        searchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override
            public boolean onQueryTextSubmit(String query) {

                return false;
            }

            @Override
            public boolean onQueryTextChange(String newText) {
                ArrayList<String> usersList = new ArrayList<>();

                for (String user : itemList){
                    if (user.toLowerCase().contains(newText.toLowerCase())){
                        usersList.add(user);
                    }
                }
                ArrayAdapter<String> adapter = new ArrayAdapter<String>(ItemListActivity.this,android.R.layout.simple_list_item_1, usersList);
                mListItems.setAdapter(adapter);


                return true;
            }
        });


        return super.onCreateOptionsMenu(menu);
    }
}
