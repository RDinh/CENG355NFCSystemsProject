package com.ceng319.partsCrib;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.SearchView;
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

public class ItemMainActivity extends AppCompatActivity{

    private RequestQueue mQueue;
    private ArrayList <String> categoryList = new ArrayList<>();
    private ListView mListCategories;
    private ArrayAdapter arrayAdapter;
    private SlidrInterface slidr;

    @Override
    protected void onCreate(Bundle savedInstanceState) {

        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_item_main);

        getSupportActionBar().setTitle(R.string.item_category);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        mListCategories = (ListView)findViewById(R.id.listViewCategories);
        mQueue = Volley.newRequestQueue(this);
        slidr = Slidr.attach(this); 

        jsonParse();
        arrayAdapter = new ArrayAdapter(this,android.R.layout.simple_list_item_1);

        mListCategories.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {

                String selectedCat = categoryList.get(i);
                Intent newIntent = new Intent(ItemMainActivity.this, ItemListActivity.class);
                newIntent.putExtra("Category",selectedCat);
                ItemMainActivity.this.startActivity(newIntent);
            }
        });
    }

    private void jsonParse(){

        String url = "http://apollo.humber.ca/~n01156096/CENG319/query.php";
        JsonObjectRequest request =  new JsonObjectRequest(Request.Method.GET, url, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        try {
                            JSONArray jsonArray = response.getJSONArray("Category");
                            for (int i = 0; i< jsonArray.length(); i++){
                                //Append to array list to create list view
                                categoryList.add(String.valueOf(jsonArray.get(i)));
                                mListCategories.setAdapter(arrayAdapter);
                                arrayAdapter.clear();
                                arrayAdapter.addAll(categoryList);
                                arrayAdapter.notifyDataSetChanged();
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

                for (String user : categoryList){
                    if (user.toLowerCase().contains(newText.toLowerCase())){
                        usersList.add(user);
                    }
                }
                ArrayAdapter<String> adapter = new ArrayAdapter<String>(ItemMainActivity.this,android.R.layout.simple_list_item_1, usersList);
                mListCategories.setAdapter(adapter);


                return true;
            }
        });


        return super.onCreateOptionsMenu(menu);
    }
}
