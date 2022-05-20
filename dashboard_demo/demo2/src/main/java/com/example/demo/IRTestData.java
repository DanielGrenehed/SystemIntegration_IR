package com.example.demo;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.sql.Timestamp;



@AllArgsConstructor
@NoArgsConstructor
@Getter
@Setter
public class IRTestData {


    private String token;
    private String code;
    private Timestamp timestamp;




}

