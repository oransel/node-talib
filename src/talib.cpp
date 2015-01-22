/*
 
               |        |         |    o|    
 ,---.,---.,---|,---.   |--- ,---.|    .|---.
 |   ||   ||   ||---'---|    ,---||    ||   |
 `   '`---'`---'`---'   `---'`---^`---'``---'
 
 NODE-TALIB Copyright (c) 2012-2014, Mustafa Oransel
 All rights reserved.
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 3 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the
 Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 Boston, MA 02111-1307, USA.
 
*/ 

#include <v8.h>
#include <node.h>

#include <unistd.h>
#include <string>

#include "./lib/include/ta_abstract.h"
#include "./lib/include/ta_common.h"
#include "./lib/include/ta_defs.h"
#include "./lib/include/ta_func.h"

using namespace node;
using namespace v8;

class Talib : ObjectWrap {
    
  private:
    
  public:

    Talib() {}
    ~Talib() {}

    // Async work object
    struct work_object {
        Persistent<Function> cb;
        TA_FuncHandle *func_handle;
        int startIdx;
        int endIdx;
        int nbOutput;
        TA_ParamHolder *func_params;
        TA_RetCode retCode;
        int outBegIdx;
        int outNBElement;
        double **outReal;
        int **outInt;
    };

    static Persistent<FunctionTemplate> persistent_function_template;

    static void Init(Handle<Object> target) { 
        
        // Define function template
        HandleScope scope;
        Local<FunctionTemplate> local_function_template = FunctionTemplate::New(New);
        persistent_function_template = Persistent<FunctionTemplate>::New(local_function_template);
        persistent_function_template->InstanceTemplate()->SetInternalFieldCount(1);
        persistent_function_template->SetClassName(String::NewSymbol("TALib"));
        
        // Define fields
        target->Set(String::New("version"), String::New("0.4.1"));
       
        // Define accessors
        target->SetAccessor(String::New("functions"), GetFunctions, NULL);

        // Initialize the engine
        TA_Initialize();
        
        // Define functions
        NODE_SET_METHOD(target, "explain", Explain);
        NODE_SET_METHOD(target, "execute", Execute);

    }
    
    static Handle<Value> New(const Arguments& args) {
        
        // Create an instance
        HandleScope scope;
        Talib* talib_instance = new Talib();
        talib_instance->Wrap(args.This());
        return args.This();
        
    }
    
    static double *V8toDoubleArray(Local<Array> array) {
        
        // Get the array length
        int length = array->Length();
        
        // Allocate memory for double array
        double *result = new double[length];
        
        // Store values in the double array
        for (int i = 0; i < length; i++) { 
            result[i] = array->Get(i)->NumberValue();
        }
        
        // Return the double array result
        return result;
        
    }
    
    static Handle<Value> Create_TA_Error(Local<Function> cb, TA_RetCode retCode) {
        
        Local<Value> argv[1];
        Local<Object> result = Object::New();
        TA_RetCodeInfo retCodeInfo;
        
        TA_SetRetCodeInfo(retCode, &retCodeInfo);
        result->Set(String::New("error"), String::New(retCodeInfo.enumStr));
        argv[0] = result;
        cb->Call(Context::GetCurrent()->Global(), 1, argv);
        return Undefined();
        
    }
    
    static Handle<Value> Create_Internal_Error(Local<Function> cb, const char *error) {
        
        Local<Value> argv[1];
        Local<Object> result = Object::New();
        result->Set(String::New("error"), String::New(error));
        argv[0] = result;
        cb->Call(Context::GetCurrent()->Global(), 1, argv);
        return Undefined();
        
    }
    
    static Handle<Value> TA_FunctionExplanation(const char *func_name) {
        
        // Function flag counter
        int func_param_flag_count;

        // Function handle and information
        const TA_FuncHandle *func_handle;
        const TA_FuncInfo   *func_info;
        
        // Function object
        Local<Object> func_object;
        Local<Object> func_param_object;
        Local<Object> func_param_flags;
        
        // Execution parameters
        Local<Array> inParams;
        Local<Array> inOptParams;
        Local<Array> outParams;
        
        // Function parameter information
        const TA_InputParameterInfo     *input_paraminfo;
        const TA_OptInputParameterInfo  *opt_paraminfo;
        const TA_OutputParameterInfo    *output_paraminfo;
        
        // Retreive the function information for the function handle
        TA_RetCode handle_retcode = TA_GetFuncHandle(func_name, &func_handle);
        TA_RetCode info_retcode = TA_GetFuncInfo(func_handle, &func_info);
        
        // Create the function object
        func_object = Object::New();
        
        // Check for error
        if ((handle_retcode != TA_SUCCESS) || (info_retcode != TA_SUCCESS))
            return func_object;
        
        // Create the execution parameters
        inParams    = Array::New();
        inOptParams = Array::New();
        outParams   = Array::New();
        
        // Store the function information
        func_object->Set(String::New("name"), String::New(func_info->name));
        func_object->Set(String::New("group"), String::New(func_info->group));
        func_object->Set(String::New("hint"), String::New(func_info->hint));
        
        // Loop for all the input parameters
        for (int i=0; i < (int)func_info->nbInput; i++) {
            
            // Get the function input parameter information
            TA_GetInputParameterInfo(func_info->handle, i, &input_paraminfo);
            
            // Create the function parameter information
            func_param_object = Object::New();
            func_param_object->Set(String::New("name"), String::New(input_paraminfo->paramName));

            // Add the function parameter type
            switch(input_paraminfo->type) {
                case TA_Input_Price: func_param_object->Set(String::New("type"), String::New("price")); break;
                case TA_Input_Real: func_param_object->Set(String::New("type"), String::New("real")); break;
                case TA_Input_Integer: func_param_object->Set(String::New("type"), String::New("integer")); break;
            }

            // Add the function parameter flags
            if (input_paraminfo->flags > 0) {

                // Create a new function flags array
                func_param_flags = Array::New();
                func_param_flag_count = 0;

                // Set the function flag defitions
                if (input_paraminfo->flags & TA_IN_PRICE_OPEN) func_param_flags->Set(func_param_flag_count++, String::New("open"));
                if (input_paraminfo->flags & TA_IN_PRICE_HIGH) func_param_flags->Set(func_param_flag_count++, String::New("high"));
                if (input_paraminfo->flags & TA_IN_PRICE_LOW) func_param_flags->Set(func_param_flag_count++, String::New("low"));
                if (input_paraminfo->flags & TA_IN_PRICE_CLOSE) func_param_flags->Set(func_param_flag_count++, String::New("close"));
                if (input_paraminfo->flags & TA_IN_PRICE_VOLUME) func_param_flags->Set(func_param_flag_count++, String::New("volume"));
                if (input_paraminfo->flags & TA_IN_PRICE_OPENINTEREST) func_param_flags->Set(func_param_flag_count++, String::New("openinterest"));
                if (input_paraminfo->flags & TA_IN_PRICE_TIMESTAMP) func_param_flags->Set(func_param_flag_count++, String::New("timestamp"));
                
                // Save the function flag definitions
                func_param_object->Set(String::New("flags"), func_param_flags);
            }
            
            // Save the function parameter
            inParams->Set(i, func_param_object);
            
        }
        
        // Loop for all the optional input parameters
        for (int i=0; i < (int)func_info->nbOptInput; i++) {
            
            // Get the function input parameter information
            TA_GetOptInputParameterInfo(func_info->handle, i, &opt_paraminfo);

            // Create the function parameter information
            func_param_object = Object::New();
            func_param_object->Set(String::New("name"), String::New(opt_paraminfo->paramName));
            func_param_object->Set(String::New("displayName"), String::New(opt_paraminfo->displayName));
            func_param_object->Set(String::New("defaultValue"), Number::New(opt_paraminfo->defaultValue));
            func_param_object->Set(String::New("hint"), String::New(opt_paraminfo->hint));

            // Add the function parameter type
            switch(opt_paraminfo->type) {
                case TA_OptInput_RealRange: func_param_object->Set(String::New("type"), String::New("real_range")); break;
                case TA_OptInput_RealList: func_param_object->Set(String::New("type"), String::New("real_list")); break;
                case TA_OptInput_IntegerRange: func_param_object->Set(String::New("type"), String::New("integer_range")); break;
                case TA_OptInput_IntegerList: func_param_object->Set(String::New("type"), String::New("integer_list")); break;
            }

            // Add the function parameter flags
            if (opt_paraminfo->flags > 0) {

                // Create a new function flags array
                func_param_flags = Array::New();
                func_param_flag_count = 0;

                // Set the function flag defitions
                if (opt_paraminfo->flags & TA_OPTIN_IS_PERCENT) func_param_flags->Set(func_param_flag_count++, String::New("percent"));
                if (opt_paraminfo->flags & TA_OPTIN_IS_DEGREE) func_param_flags->Set(func_param_flag_count++, String::New("degree"));
                if (opt_paraminfo->flags & TA_OPTIN_IS_CURRENCY) func_param_flags->Set(func_param_flag_count++, String::New("currency"));
                if (opt_paraminfo->flags & TA_OPTIN_ADVANCED) func_param_flags->Set(func_param_flag_count++, String::New("advanced"));
                
                // Save the function flag definitions
                func_param_object->Set(String::New("flags"), func_param_flags);
            }
            
            // Save the function parameter
            inOptParams->Set(i, func_param_object);
            
        }
        
        // Loop for all the output parameters
        for (int i=0; i < (int)func_info->nbOutput; i++) {
            
            // Get the function input parameter information
            TA_GetOutputParameterInfo(func_info->handle, i, &output_paraminfo);

            // Create the function parameter information
            func_param_object = Object::New();
            func_param_object->Set(String::New("name"), String::New(output_paraminfo->paramName));

            // Add the function parameter type
            switch(output_paraminfo->type) {
                case TA_Output_Real: func_param_object->Set(String::New("type"), String::New("real")); break;
                case TA_Output_Integer: func_param_object->Set(String::New("type"), String::New("integer")); break;
            }

            // Add the function parameter flags
            if (output_paraminfo->flags > 0) {

                // Create a new function flags array
                func_param_flags = Array::New();
                func_param_flag_count = 0;

                // Set the function flag defitions
                if (output_paraminfo->flags & TA_OUT_LINE) func_param_flags->Set(func_param_flag_count++, String::New("line"));
                if (output_paraminfo->flags & TA_OUT_DOT_LINE) func_param_flags->Set(func_param_flag_count++, String::New("line_dot"));
                if (output_paraminfo->flags & TA_OUT_DASH_LINE) func_param_flags->Set(func_param_flag_count++, String::New("line_dash"));
                if (output_paraminfo->flags & TA_OUT_DOT) func_param_flags->Set(func_param_flag_count++, String::New("dot"));
                if (output_paraminfo->flags & TA_OUT_HISTO) func_param_flags->Set(func_param_flag_count++, String::New("histogram"));
                if (output_paraminfo->flags & TA_OUT_PATTERN_BOOL) func_param_flags->Set(func_param_flag_count++, String::New("pattern_bool"));
                if (output_paraminfo->flags & TA_OUT_PATTERN_BULL_BEAR) func_param_flags->Set(func_param_flag_count++, String::New("pattern_bull_bear"));
                if (output_paraminfo->flags & TA_OUT_PATTERN_STRENGTH) func_param_flags->Set(func_param_flag_count++, String::New("pattern_strength"));
                if (output_paraminfo->flags & TA_OUT_POSITIVE) func_param_flags->Set(func_param_flag_count++, String::New("positive"));
                if (output_paraminfo->flags & TA_OUT_NEGATIVE) func_param_flags->Set(func_param_flag_count++, String::New("negative"));
                if (output_paraminfo->flags & TA_OUT_ZERO) func_param_flags->Set(func_param_flag_count++, String::New("zero"));
                if (output_paraminfo->flags & TA_OUT_UPPER_LIMIT) func_param_flags->Set(func_param_flag_count++, String::New("limit_upper"));
                if (output_paraminfo->flags & TA_OUT_LOWER_LIMIT) func_param_flags->Set(func_param_flag_count++, String::New("limit_lower"));
                
                // Save the function flag definitions
                func_param_object->Set(String::New("flags"), func_param_flags);
            }
            
            // Save the function parameter
            outParams->Set(i, func_param_object);
            
        }
        
        // Store the function parameters
        func_object->Set(String::New("inputs"), inParams);
        func_object->Set(String::New("optInputs"), inOptParams);
        func_object->Set(String::New("outputs"), outParams);
        
        return func_object;
        
    }
    
    static Handle<Value> GetFunctions(v8::Local<v8::String> property, const v8::AccessorInfo& info) {
        
        // Function group table
        TA_StringTable *group_table;
        
        // Function table
        TA_StringTable *func_table;
        
        // Function name array
        Local<Array> func_array = Array::New();
        
        // Function count
        int func_count = 0;
        
        // Get the function groups
        if (TA_GroupTableAlloc(&group_table) == TA_SUCCESS) {
            
            // Loop for all the function groups
            for (unsigned int group_index=0; group_index < group_table->size; group_index++) {
                
                // Get the functions in the group
                if (TA_FuncTableAlloc(group_table->string[group_index], &func_table) == TA_SUCCESS) {
                    
                    // Loop for all the functions
                    for (unsigned int func_index=0; func_index < func_table->size; func_index++) {
                        
                        // Save the function name to the array
                        func_array->Set(func_count++, TA_FunctionExplanation(func_table->string[func_index]));
                        
                    }
                    
                    // Clear function table memory
                    TA_FuncTableFree(func_table);
                }
                
            }
            
            // Clear function group table memory
            TA_GroupTableFree(group_table);
            
        }
        
        // Return function names
        return func_array;
        
    }
    
    static Handle<Value> Explain(const Arguments& args) {
        
        // Check the arguments
        if (args.Length() < 1)
            return ThrowException(Exception::TypeError(String::New("One argument required - Function name")));
        
        // Check the function name parameter
        if (!args[0]->IsString())
            return ThrowException(Exception::TypeError(String::New("First argument must be a String")));
        
        // Retreive the function name string
        String::AsciiValue func_name(args[0]->ToString());
        
        return TA_FunctionExplanation(*func_name);
    }
    
    static Handle<Value> Execute(const Arguments& args) {
        
        HandleScope scope;
    
        // Execution parameter
        Local<Object> executeParameter;
        
        // Callback function
        Local<Function> cb;
        
        // Price values
        double *open            = NULL;
        double *close           = NULL;
        double *high            = NULL;
        double *low             = NULL;
        double *volume          = NULL;
        double *openInterest    = NULL;
        
        double *inRealList		= NULL;
        
        // Numeric values
        double inReal;
        int inInteger;
        
        // Return code
        TA_RetCode retCode;
        
        // Function handle
        const TA_FuncHandle *func_handle;
        
        // Function information
        const TA_FuncInfo *func_info;
        
        // Function parameter holder
        TA_ParamHolder *func_params;
        
        // Function parameter information
        const TA_InputParameterInfo     *input_paraminfo;
        const TA_OptInputParameterInfo  *opt_paraminfo;
        const TA_OutputParameterInfo    *output_paraminfo;
        
        // Check the arguments
        if (args.Length() < 2)
            return ThrowException(Exception::TypeError(String::New("Two arguments required - Object and Function")));
        
        // Check the execution parameter
        if (!args[0]->IsObject())
            return ThrowException(Exception::TypeError(String::New("First argument must be an Object with fields name, startIdx, endIdx and function input parameters")));
        
        // Check the callback parameter
        if (!args[1]->IsFunction())
            return ThrowException(Exception::TypeError(String::New("Second argument must be a Function")));
        
        // Get the execute parameter
        executeParameter = args[0]->ToObject();
        
        // Get the callback function
        cb = Local<Function>::Cast(args[1]);
        
        // Check the function name parameter
        if (!executeParameter->HasOwnProperty(String::New("name")))
            return Create_Internal_Error(cb, "First argument must contain 'name' field");
        
        // Retreive the function name string
        String::AsciiValue func_name(executeParameter->Get(String::New("name")));
        
        // Check the start index
        if (!executeParameter->HasOwnProperty(String::New("startIdx")))
            return Create_Internal_Error(cb, "First argument must contain 'startIdx' field");
        
        // Check the end index
        if (!executeParameter->HasOwnProperty(String::New("endIdx")))
            return Create_Internal_Error(cb, "First argument must contain 'endIdx' field");
        
        // Refreive the start and end index
        int startIdx = executeParameter->Get(String::New("startIdx"))->Int32Value();
        int endIdx = executeParameter->Get(String::New("endIdx"))->Int32Value();

        // Retreive the function handle for function name
        if ((retCode = TA_GetFuncHandle(*func_name, &func_handle)) != TA_SUCCESS)
            return Create_TA_Error(cb, retCode);
        
        // Retreive the function information for the function handle
        if ((retCode = TA_GetFuncInfo(func_handle, &func_info)) != TA_SUCCESS)
            return Create_TA_Error(cb, retCode);

        // Allocate parameter holder memory for function handle
        if ((retCode = TA_ParamHolderAlloc(func_handle, &func_params)) != TA_SUCCESS)
            return Create_TA_Error(cb, retCode);
        
        // Loop for all the input parameters
        for (int i=0; i < (int)func_info->nbInput; i++) {
            
            // Get the function input parameter information
            TA_GetInputParameterInfo(func_info->handle, i, &input_paraminfo);
            
            // Check the input parameter type
            switch (input_paraminfo->type) {
                
                // Input parameter price type is needed
                case TA_Input_Price:
                    
                    // Check if the parameter info requires open price
                    if (input_paraminfo->flags & TA_IN_PRICE_OPEN) {
                        
                        // Check if the price object has open prices
                        if (!executeParameter->HasOwnProperty(String::New("open"))) {
                            
                            // Clear parameter holder memory
                            TA_ParamHolderFree(func_params);
                            
                            // Return internal error
                            return Create_Internal_Error(cb, "First argument must contain 'open' field");
                            
                            
                        }
                        
                        // Get the open prices
                        open = V8toDoubleArray(Local<Array>::Cast(executeParameter->Get(String::New("open"))));
                        
                    }
                    
                    // Check if the parameter info requires high price
                    if (input_paraminfo->flags & TA_IN_PRICE_HIGH) {
                        
                        // Check if the price object has high prices
                        if (!executeParameter->HasOwnProperty(String::New("high"))) {
                            
                            // Clear parameter holder memory
                            TA_ParamHolderFree(func_params);

                            // Clear values memory
                            if (open)
                                delete[] open;
                            
                            // Return internal error
                            return Create_Internal_Error(cb, "First argument must contain 'high' field");
                            
                        }
                        
                        // Get the high prices
                        high = V8toDoubleArray(Local<Array>::Cast(executeParameter->Get(String::New("high"))));
                        
                    }
                    
                    // Check if the parameter info requires low price
                    if (input_paraminfo->flags & TA_IN_PRICE_LOW) {
                        
                        // Check if the price object has low prices
                        if (!executeParameter->HasOwnProperty(String::New("low"))) {
                            
                            // Clear parameter holder memory
                            TA_ParamHolderFree(func_params);
                            
                            // Clear values memory
                            if (open)
                                delete[] open;
                            if (high)
                                delete[] high;
                            
                            // Return internal error
                            return Create_Internal_Error(cb, "First argument must contain 'low' field");
                            
                        }
                        
                        // Get the low prices
                        low = V8toDoubleArray(Local<Array>::Cast(executeParameter->Get(String::New("low"))));
                        
                    }
                    
                    // Check if the parameter info requires close price
                    if (input_paraminfo->flags & TA_IN_PRICE_CLOSE) {
                        
                        // Check if the price object has close prices
                        if (!executeParameter->HasOwnProperty(String::New("close"))) {
                            
                            // Clear parameter holder memory
                            TA_ParamHolderFree(func_params);
                            
                            // Clear values memory
                            if (open)
                                delete[] open;
                            if (low)
                                delete[] low;
                            if (high)
                                delete[] high;
                            
                            // Return internal error
                            return Create_Internal_Error(cb, "First argument must contain 'close' field");
                            
                        }
                        
                        // Get the close prices
                        close = V8toDoubleArray(Local<Array>::Cast(executeParameter->Get(String::New("close"))));
                        
                    }
                    
                    // Check if the parameter info requires volume
                    if (input_paraminfo->flags & TA_IN_PRICE_VOLUME) {
                        
                        // Check if the price object has volume
                        if (!executeParameter->HasOwnProperty(String::New("volume"))) {
                            
                            // Clear parameter holder memory
                            TA_ParamHolderFree(func_params);

                            // Clear values memory
                            if (open)
                                delete[] open;
                            if (close)
                                delete[] close;
                            if (low)
                                delete[] low;
                            if (high)
                                delete[] high;
                            
                            // Return internal error
                            return Create_Internal_Error(cb, "First argument must contain 'volume' field");
                            
                        }
                        
                        // Get the volume
                        volume = V8toDoubleArray(Local<Array>::Cast(executeParameter->Get(String::New("volume"))));
                        
                    }
                    
                    // Check if the parameter info requires open interest values
                    if (input_paraminfo->flags & TA_IN_PRICE_OPENINTEREST) {
                        
                        // Check if the price object has open interest values
                        if (!executeParameter->HasOwnProperty(String::New("openInterest"))) {
                            
                            // Clear parameter holder memory
                            TA_ParamHolderFree(func_params);
                            
                            // Clear values memory
                            if (close)
                                delete[] close;
                            if (volume)
                                delete[] volume;
                            if (high)
                                delete[] high;
                            if (low)
                                delete[] low;
                            
                            // Return internal error
                            return Create_Internal_Error(cb, "First argument must contain 'openInterest' field");
                            
                        }
                        
                        // Get the open interest values
                        openInterest = V8toDoubleArray(Local<Array>::Cast(executeParameter->Get(String::New("openInterest"))));
                        
                    }
                    
                    // Save the price parameters
                    if ((retCode = TA_SetInputParamPricePtr(func_params, i, open, high, low, close, volume, openInterest)) != TA_SUCCESS) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);
                        
                        // Return TA error
                        return Create_TA_Error(cb, retCode);
                        
                    }
                    
                    break;
                
                // Input parameter real type is needed
                case TA_Input_Real:
                    
                    // Check if the input parameter object has real value
                    if (!executeParameter->HasOwnProperty(String::New(input_paraminfo->paramName))) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);
                        
                        // Return internal error
                        return Create_Internal_Error(cb, ((std::string)("First argument must contain '") + (std::string)(input_paraminfo->paramName) + (std::string)("' field")).c_str());
                        
                    }
                    
                    // Get the number parameter value
                    inRealList = V8toDoubleArray(Local<Array>::Cast(executeParameter->Get(String::New(input_paraminfo->paramName))));
                     
                    // Save the number parameter
                    if ((retCode = TA_SetInputParamRealPtr(func_params, i, inRealList)) != TA_SUCCESS) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);
                        
                        // Return TA error
                        return Create_TA_Error(cb, retCode);
                        
                    }
                    
                    break;
                
                // Input parameter integer type is needed
                case TA_Input_Integer:
                    
                    // Check if the input parameter object has integer parameter
                    if (!executeParameter->HasOwnProperty(String::New(input_paraminfo->paramName))) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);
                        
                        // Return internal error
                        return Create_Internal_Error(cb, ((std::string)("First argument must contain '") + (std::string)(input_paraminfo->paramName) + (std::string)("' field")).c_str());
                        
                    }
                    
                    // Get the integer parameter value
                    inInteger = executeParameter->Get(String::New(input_paraminfo->paramName))->IntegerValue();
                    
                    // Save the integer parameter
                    if ((retCode = TA_SetInputParamIntegerPtr(func_params, i, &inInteger)) != TA_SUCCESS) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);
                        
                        // Return TA error
                        return Create_TA_Error(cb, retCode);
                        
                    }
                    
                    break;
            }
            
        }
        
        // Loop for all the optional input parameters
        for (int i=0; i < (int)func_info->nbOptInput; i++) {
            
            // Get the optional input parameter information
            TA_GetOptInputParameterInfo(func_info->handle, i, &opt_paraminfo);
            
            // Check if the optional parameter object has real value
            if (!executeParameter->HasOwnProperty(String::New(opt_paraminfo->paramName))) {
                
                // Clear parameter holder memory
                TA_ParamHolderFree(func_params);
                
                // Return internal error
                return Create_Internal_Error(cb, ((std::string)("First argument must contain '") + (std::string)(opt_paraminfo->paramName) + (std::string)("' field")).c_str());
                
            }
            
            // Check the optional input parameter type
            switch (opt_paraminfo->type) {
                    
                case TA_OptInput_RealRange:
                case TA_OptInput_RealList:
                    
                    // Get the integer parameter value
                    inReal = executeParameter->Get(String::New(opt_paraminfo->paramName))->NumberValue();
                    
                    // Save the integer parameter
                    if ((retCode = TA_SetOptInputParamReal(func_params, i, inReal)) != TA_SUCCESS) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);
                        
                        // Return TA error
                        return Create_TA_Error(cb, retCode);
                        
                    }
                    
                    break;
                case TA_OptInput_IntegerRange:
                case TA_OptInput_IntegerList:
                    
                    // Get the integer parameter value
                    inInteger = executeParameter->Get(String::New(opt_paraminfo->paramName))->IntegerValue();
                    
                    // Save the integer parameter
                    if ((retCode = TA_SetOptInputParamInteger(func_params, i, inInteger)) != TA_SUCCESS) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);
                        
                        // Return TA error
                        return Create_TA_Error(cb, retCode);
                        
                    }
                    
                    break;
                    
            }
            
        }
        
        // Setup the work object
        work_object *wo = new work_object();
        wo->func_handle = (TA_FuncHandle *)func_handle;
        wo->startIdx = startIdx;
        wo->endIdx = endIdx;
        wo->nbOutput = func_info->nbOutput;
        wo->func_params = func_params;
        wo->outReal = new double*[func_info->nbOutput];
        wo->outInt = new int*[func_info->nbOutput];
        wo->cb = Persistent<Function>::New(cb);
        
        // Loop for all the ouput parameters
        for (int i=0; i < wo->nbOutput; i++) {
            
            // Allocate memory
            wo->outReal[i] = new double[endIdx-startIdx+1];
            wo->outInt[i] = new int[endIdx-startIdx+1];
            
            // Clear output results memory
            for (int x=0; x < (endIdx-startIdx+1); x++) {
                wo->outReal[i][x] = 0;
                wo->outInt[i][x] = 0;
            }
            
            // Get the output parameter information
            TA_GetOutputParameterInfo(func_info->handle, i, &output_paraminfo);
            
            // Check the output parameter type
            switch(output_paraminfo->type) {
                
                // Output parameter type real is needed
                case TA_Output_Real:
                    
                    // Assign the real parameter memory
                    TA_SetOutputParamRealPtr(func_params, i, &wo->outReal[i][0]);
                    
                    break;
                
                // Output parameter type integer is needed
                case TA_Output_Integer:
                    
                    // Assign the integer parameter memory
                    TA_SetOutputParamIntegerPtr(func_params, i, &wo->outInt[i][0]);
                    
                    break;
                    
            }
            
        }
        
        uv_work_t *req = new uv_work_t;
        req->data = wo;

        // Queue the work
        uv_queue_work(uv_default_loop(), req, ExecuteWork, ExecuteWorkDone);
        
        return Undefined();
        
    }
    
    static void ExecuteWork(uv_work_t *req) {
        
        // Get the work object
        work_object *wo = static_cast<work_object *>(req->data);
        
        // Execute the function call with parameters declared
        wo->retCode = TA_CallFunc((const TA_ParamHolder *)wo->func_params, wo->startIdx, wo->endIdx, &wo->outBegIdx, &wo->outNBElement);
        
    }
    
    static void ExecuteWorkDone(uv_work_t *req, int status) {
        
        HandleScope scope;
        
        work_object *wo = static_cast<work_object *>(req->data);

        // Create the outputs object
        Local<Object> outputArray = Object::New();
        
        // Execution result object 
        Local<Object> result = Object::New();
        
        // Result args
        Local<Value> argv[1];
        
        // Determine the number of results
        int resultLength = wo->outNBElement;
        
        // Function output parameter information
        const TA_OutputParameterInfo *output_paraminfo;
        
        // Check for execution error
        if (wo->retCode != TA_SUCCESS) {
            //Create_TA_Error(wo->cb, wo->retCode);
            goto execute_exit;
        }
        
        // Set beginning index and number of elements
        result->Set(String::New("begIndex"), Number::New(wo->outBegIdx));
        result->Set(String::New("nbElement"), Number::New(wo->outNBElement));
        
        // Loop for all the output parameters
        for (int i=0; i < wo->nbOutput; i++) {
            
            // Get the output parameter information
            TA_GetOutputParameterInfo(wo->func_handle, i, &output_paraminfo);
            
            // Create an array for results
            Local<Array> resultArray = Array::New(resultLength);
            
            // Loop for all the results
            for (int x = 0; x < resultLength; x++) {
                
                // Determine the output type
                switch(output_paraminfo->type) {
                        
                        // Output type real is needed
                    case TA_Output_Real:
                        
                        // Set the real output value
                        resultArray->Set(x, Number::New(wo->outReal[i][x]));
                        
                        break;
                        
                        // Output type integer is needed
                    case TA_Output_Integer:
                        
                        // Set the integer output value
                        resultArray->Set(x, Number::New(wo->outInt[i][x]));
                        
                        break;
                }
                
            }
            
            // Set the result array
            outputArray->Set(String::New(output_paraminfo->paramName), resultArray);
            
        }
        
        // Set the outputs array
        result->Set(String::New("result"), outputArray);
        
        // Return the execution result
        argv[0] = result;
        wo->cb->Call(Context::GetCurrent()->Global(), 1, argv);

    execute_exit:
        
        // Clear parameter holder memory
        TA_ParamHolderFree(wo->func_params);
        
        // Dispose callback
        wo->cb.Dispose();
        
        // Dispose output arrays
        for (int i=0; i < wo->nbOutput; i++) {
            delete wo->outReal[i];
            delete wo->outInt[i];
        }
        delete wo->outReal;
        delete wo->outInt;
        
        // Dispose work object
        delete wo;
        
        return delete req;
    }
    
};

Persistent<FunctionTemplate> Talib::persistent_function_template;
extern "C" {
  static void init (Handle<Object> target) {
    Talib::Init(target);
  }

  NODE_MODULE(talib, init);
}
