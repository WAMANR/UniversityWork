package com.example.securedchat.listeners;

import com.example.securedchat.models.User;

import java.security.NoSuchAlgorithmException;

public interface UserListerner {

    void onUserClicked(User user) throws NoSuchAlgorithmException;
}
