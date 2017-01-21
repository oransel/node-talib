/*
 
               |        |         |    o|    
 ,---.,---.,---|,---.   |--- ,---.|    .|---.
 |   ||   ||   ||---'---|    ,---||    ||   |
 `   '`---'`---'`---'   `---'`---^`---'``---'
 
 NODE-TALIB Copyright (c) 2012-2015, Mustafa Oransel
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

#include <node.h>
#include <nan.h>

#include "./lib/include/ta_abstract.h"
#include "./lib/include/ta_common.h"
#include "./lib/include/ta_defs.h"
#include "./lib/include/ta_func.h"

using v8::Function;
using v8::FunctionTemplate;
using v8::ObjectTemplate;
using v8::Handle;
using v8::Object;
using v8::Local;
using v8::MaybeLocal;
using v8::Value;
using v8::String;
using v8::Number;
using v8::Array;
using Nan::GetFunction;
using Nan::Callback;
using Nan::New;
using Nan::Set;
using Nan::Get;
using Nan::HandleScope;
using Nan::ReturnValue;
using Nan::ThrowTypeError;
using Nan::Utf8String;
using Nan::To;
using Nan::AsyncWorker;
using Nan::HasOwnProperty;

// Async work object
struct work_object {
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
    double **garbage;
    int garbage_count;
};

static void REPORT_TA_ERROR(Callback *callback, TA_RetCode retCode) {

    // Report TA Error
    Local<Value> argv[1];
    Local<Object> result = New<Object>();
    TA_RetCodeInfo retCodeInfo;
    TA_SetRetCodeInfo(retCode, &retCodeInfo);
    Set(result, New<String>("error").ToLocalChecked(), New<String>(retCodeInfo.enumStr).ToLocalChecked());
    argv[0] = result;
    callback->Call(1, argv);
}

static void REPORT_INTERNAL_ERROR(Callback *callback, const char *error) {

    // Report Internal Error
    Local<Value> argv[1];
    Local<Object> result = New<Object>();
    Set(result, New<String>("error").ToLocalChecked(), New<String>(error).ToLocalChecked());
    argv[0] = result;
    callback->Call(1, argv);
}

static double *V8_TO_DOUBLE_ARRAY(Local<Array> array) {
        
    // Get the array length
    int length = array->Length();
    
    // Allocate memory for double array
    double *result = new double[length];
    
    // Store values in the double array
    for (int i = 0; i < length; i++) {
        result[i] = Get(array, i).ToLocalChecked()->NumberValue();
    }
    
    // Return the double array result
    return result;
    
}

static Handle<Value> TA_EXPLAIN_FUNCTION(const char *func_name) {
        
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
    func_object = New<Object>();
    
    // Check for error
    if ((handle_retcode != TA_SUCCESS) || (info_retcode != TA_SUCCESS))
        return func_object;
    
    // Create the execution parameters
    inParams    = New<Array>();
    inOptParams = New<Array>();
    outParams   = New<Array>();
    
    // Store the function information
    Set(func_object, New("name").ToLocalChecked(), New(func_info->name).ToLocalChecked());
    Set(func_object, New("group").ToLocalChecked(), New(func_info->group).ToLocalChecked());
    Set(func_object, New("hint").ToLocalChecked(), New(func_info->hint).ToLocalChecked());
    
    // Loop for all the input parameters
    for (int i=0; i < (int)func_info->nbInput; i++) {
        
        // Get the function input parameter information
        TA_GetInputParameterInfo(func_info->handle, i, &input_paraminfo);
        
        // Create the function parameter information
        func_param_object = New<Object>();
        Set(func_param_object, New("name").ToLocalChecked(), New(input_paraminfo->paramName).ToLocalChecked());

        // Add the function parameter type
        switch(input_paraminfo->type) {
            case TA_Input_Price: Set(func_param_object, New("type").ToLocalChecked(), New("price").ToLocalChecked()); break;
            case TA_Input_Real: Set(func_param_object, New("type").ToLocalChecked(), New("real").ToLocalChecked()); break;
            case TA_Input_Integer: Set(func_param_object, New("type").ToLocalChecked(), New("integer").ToLocalChecked()); break;
        }

        // Add the function parameter flags
        if (input_paraminfo->flags > 0) {

            // Create a new function flags array
            func_param_flags = New<Object>();
            func_param_flag_count = 0;

            // Set the function flag defitions
            if (input_paraminfo->flags & TA_IN_PRICE_OPEN) Set(func_param_flags, func_param_flag_count++, New("open").ToLocalChecked());
            if (input_paraminfo->flags & TA_IN_PRICE_HIGH) Set(func_param_flags, func_param_flag_count++, New("high").ToLocalChecked());
            if (input_paraminfo->flags & TA_IN_PRICE_LOW) Set(func_param_flags, func_param_flag_count++, New("low").ToLocalChecked());
            if (input_paraminfo->flags & TA_IN_PRICE_CLOSE) Set(func_param_flags, func_param_flag_count++, New("close").ToLocalChecked());
            if (input_paraminfo->flags & TA_IN_PRICE_VOLUME) Set(func_param_flags, func_param_flag_count++, New("volume").ToLocalChecked());
            if (input_paraminfo->flags & TA_IN_PRICE_OPENINTEREST) Set(func_param_flags, func_param_flag_count++, New("openinterest").ToLocalChecked());
            if (input_paraminfo->flags & TA_IN_PRICE_TIMESTAMP) Set(func_param_flags, func_param_flag_count++, New("timestamp").ToLocalChecked());
            
            // Save the function flag definitions
            Set(func_param_object, New("flags").ToLocalChecked(), func_param_flags);
        }
        
        // Save the function parameter
        Set(inParams, i, func_param_object);
        
    }
    
    // Loop for all the optional input parameters
    for (int i=0; i < (int)func_info->nbOptInput; i++) {
        
        // Get the function input parameter information
        TA_GetOptInputParameterInfo(func_info->handle, i, &opt_paraminfo);

        // Create the function parameter information
        func_param_object = New<Object>();
        Set(func_param_object, New<String>("name").ToLocalChecked(), New(opt_paraminfo->paramName).ToLocalChecked());
        Set(func_param_object, New<String>("displayName").ToLocalChecked(), New(opt_paraminfo->displayName).ToLocalChecked());
        Set(func_param_object, New<String>("defaultValue").ToLocalChecked(), New(opt_paraminfo->defaultValue));
        Set(func_param_object, New<String>("hint").ToLocalChecked(), New(opt_paraminfo->hint).ToLocalChecked());

        // Add the function parameter type
        switch(opt_paraminfo->type) {
            case TA_OptInput_RealRange: Set(func_param_object, New<String>("type").ToLocalChecked(), New<String>("real_range").ToLocalChecked()); break;
            case TA_OptInput_RealList: Set(func_param_object, New<String>("type").ToLocalChecked(), New<String>("real_list").ToLocalChecked()); break;
            case TA_OptInput_IntegerRange: Set(func_param_object, New<String>("type").ToLocalChecked(), New<String>("integer_range").ToLocalChecked()); break;
            case TA_OptInput_IntegerList: Set(func_param_object, New<String>("type").ToLocalChecked(), New<String>("integer_list").ToLocalChecked()); break;
        }

        // Add the function parameter flags
        if (opt_paraminfo->flags > 0) {

            // Create a new function flags array
            func_param_flags = New<Object>();
            func_param_flag_count = 0;

            // Set the function flag defitions
            if (opt_paraminfo->flags & TA_OPTIN_IS_PERCENT) Set(func_param_object, func_param_flag_count++, New<String>("percent").ToLocalChecked());
            if (opt_paraminfo->flags & TA_OPTIN_IS_DEGREE) Set(func_param_object, func_param_flag_count++, New<String>("degree").ToLocalChecked());
            if (opt_paraminfo->flags & TA_OPTIN_IS_CURRENCY) Set(func_param_object, func_param_flag_count++, New<String>("currency").ToLocalChecked());
            if (opt_paraminfo->flags & TA_OPTIN_ADVANCED) Set(func_param_object, func_param_flag_count++, New<String>("advanced").ToLocalChecked());
            
            // Save the function flag definitions
            Set(func_param_object, New<String>("flags").ToLocalChecked(), func_param_flags);
        }
        
        // Save the function parameter
        Set(inOptParams, i, func_param_object);
        
    }
    
    // Loop for all the output parameters
    for (int i=0; i < (int)func_info->nbOutput; i++) {
        
        // Get the function input parameter information
        TA_GetOutputParameterInfo(func_info->handle, i, &output_paraminfo);

        // Create the function parameter information
        func_param_object = New<Object>();
        Set(func_param_object, New<String>("name").ToLocalChecked(), New(output_paraminfo->paramName).ToLocalChecked());

        // Add the function parameter type
        switch(output_paraminfo->type) {
            case TA_Output_Real: Set(func_param_object, New<String>("type").ToLocalChecked(), New<String>("real").ToLocalChecked()); break;
            case TA_Output_Integer: Set(func_param_object, New<String>("type").ToLocalChecked(), New<String>("integer").ToLocalChecked()); break;
        }

        // Add the function parameter flags
        if (output_paraminfo->flags > 0) {

            // Create a new function flags array
            func_param_flags = New<Object>();
            func_param_flag_count = 0;

            // Set the function flag defitions
            if (output_paraminfo->flags & TA_OUT_LINE) Set(func_param_object, func_param_flag_count++, New<String>("line").ToLocalChecked());
            if (output_paraminfo->flags & TA_OUT_DOT_LINE) Set(func_param_object, func_param_flag_count++, New<String>("line_dot").ToLocalChecked());
            if (output_paraminfo->flags & TA_OUT_DASH_LINE) Set(func_param_object, func_param_flag_count++, New<String>("line_dash").ToLocalChecked());
            if (output_paraminfo->flags & TA_OUT_DOT) Set(func_param_object, func_param_flag_count++, New<String>("dot").ToLocalChecked());
            if (output_paraminfo->flags & TA_OUT_HISTO) Set(func_param_object, func_param_flag_count++, New<String>("histogram").ToLocalChecked());
            if (output_paraminfo->flags & TA_OUT_PATTERN_BOOL) Set(func_param_object, func_param_flag_count++, New<String>("pattern_bool").ToLocalChecked());
            if (output_paraminfo->flags & TA_OUT_PATTERN_BULL_BEAR) Set(func_param_object, func_param_flag_count++, New<String>("pattern_bull_bear").ToLocalChecked());
            if (output_paraminfo->flags & TA_OUT_PATTERN_STRENGTH) Set(func_param_object, func_param_flag_count++, New<String>("pattern_strength").ToLocalChecked());
            if (output_paraminfo->flags & TA_OUT_POSITIVE) Set(func_param_object, func_param_flag_count++, New<String>("positive").ToLocalChecked());
            if (output_paraminfo->flags & TA_OUT_NEGATIVE) Set(func_param_object, func_param_flag_count++, New<String>("negative").ToLocalChecked());
            if (output_paraminfo->flags & TA_OUT_ZERO) Set(func_param_object, func_param_flag_count++, New<String>("zero").ToLocalChecked());
            if (output_paraminfo->flags & TA_OUT_UPPER_LIMIT) Set(func_param_object, func_param_flag_count++, New<String>("limit_upper").ToLocalChecked());
            if (output_paraminfo->flags & TA_OUT_LOWER_LIMIT) Set(func_param_object, func_param_flag_count++, New<String>("limit_lower").ToLocalChecked());
            
            // Save the function flag definitions
            Set(func_param_object, New<String>("flags").ToLocalChecked(), func_param_flags);
        }
        
        // Save the function parameter
        Set(outParams, i, func_param_object);
        
    }
    
    // Store the function parameters
    Set(func_object, New<String>("inputs").ToLocalChecked(), inParams);
    Set(func_object, New<String>("optInputs").ToLocalChecked(), inOptParams);
    Set(func_object, New<String>("outputs").ToLocalChecked(), outParams);
    
    return func_object;
    
}

NAN_GETTER(Functions) {
    HandleScope();

    // Function group table
    TA_StringTable *group_table;
    
    // Function table
    TA_StringTable *func_table;

    // Function name array
    Local<Array> func_array = New<Array>();

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
                    Set(func_array, func_count++, TA_EXPLAIN_FUNCTION(func_table->string[func_index]));
                    
                }
                
                // Clear function table memory
                TA_FuncTableFree(func_table);
            }
            
        }
        
        // Clear function group table memory
        TA_GroupTableFree(group_table);
        
    }

    // Return function names
    info.GetReturnValue().Set(func_array);
}

NAN_METHOD(Explain) {
    HandleScope();

    // Check if any arguments are passed
    if (info.Length() < 1) {
        ThrowTypeError("One argument required - Function name");
        return;
    }

    // Check if first parameter is a string
    if (!info[0]->IsString()) {
        ThrowTypeError("First argument must be a String");
        return;
    }

    // Retreive the function name string
    Utf8String func_name(info[0]->ToString());
    
    info.GetReturnValue().Set(TA_EXPLAIN_FUNCTION(*func_name));
}

NAN_GETTER(FunctionUnstIds) {

    // Function object
    Local<Object> func_object = New<Object>();

    // Add Function Unstable IDs
    Set(func_object, New<String>("TA_FUNC_UNST_ADX").ToLocalChecked(), New(TA_FUNC_UNST_ADX));
    Set(func_object, New<String>("TA_FUNC_UNST_ADXR").ToLocalChecked(), New(TA_FUNC_UNST_ADXR));
    Set(func_object, New<String>("TA_FUNC_UNST_ATR").ToLocalChecked(), New(TA_FUNC_UNST_ATR));
    Set(func_object, New<String>("TA_FUNC_UNST_CMO").ToLocalChecked(), New(TA_FUNC_UNST_CMO));
    Set(func_object, New<String>("TA_FUNC_UNST_DX").ToLocalChecked(), New(TA_FUNC_UNST_DX));
    Set(func_object, New<String>("TA_FUNC_UNST_EMA").ToLocalChecked(), New(TA_FUNC_UNST_EMA));
    Set(func_object, New<String>("TA_FUNC_UNST_HT_DCPERIOD").ToLocalChecked(), New(TA_FUNC_UNST_HT_DCPERIOD));
    Set(func_object, New<String>("TA_FUNC_UNST_HT_DCPHASE").ToLocalChecked(), New(TA_FUNC_UNST_HT_DCPHASE));
    Set(func_object, New<String>("TA_FUNC_UNST_HT_PHASOR").ToLocalChecked(), New(TA_FUNC_UNST_HT_PHASOR));
    Set(func_object, New<String>("TA_FUNC_UNST_HT_SINE").ToLocalChecked(), New(TA_FUNC_UNST_HT_SINE));
    Set(func_object, New<String>("TA_FUNC_UNST_HT_TRENDLINE").ToLocalChecked(), New(TA_FUNC_UNST_HT_TRENDLINE));
    Set(func_object, New<String>("TA_FUNC_UNST_HT_TRENDMODE").ToLocalChecked(), New(TA_FUNC_UNST_HT_TRENDMODE));
    Set(func_object, New<String>("TA_FUNC_UNST_IMI").ToLocalChecked(), New(TA_FUNC_UNST_IMI));
    Set(func_object, New<String>("TA_FUNC_UNST_KAMA").ToLocalChecked(), New(TA_FUNC_UNST_KAMA));
    Set(func_object, New<String>("TA_FUNC_UNST_MAMA").ToLocalChecked(), New(TA_FUNC_UNST_MAMA));
    Set(func_object, New<String>("TA_FUNC_UNST_MFI").ToLocalChecked(), New(TA_FUNC_UNST_MFI));
    Set(func_object, New<String>("TA_FUNC_UNST_MINUS_DI").ToLocalChecked(), New(TA_FUNC_UNST_MINUS_DI));
    Set(func_object, New<String>("TA_FUNC_UNST_MINUS_DM").ToLocalChecked(), New(TA_FUNC_UNST_MINUS_DM));
    Set(func_object, New<String>("TA_FUNC_UNST_NATR").ToLocalChecked(), New(TA_FUNC_UNST_NATR));
    Set(func_object, New<String>("TA_FUNC_UNST_PLUS_DI").ToLocalChecked(), New(TA_FUNC_UNST_PLUS_DI));
    Set(func_object, New<String>("TA_FUNC_UNST_PLUS_DM").ToLocalChecked(), New(TA_FUNC_UNST_PLUS_DM));
    Set(func_object, New<String>("TA_FUNC_UNST_RSI").ToLocalChecked(), New(TA_FUNC_UNST_RSI));
    Set(func_object, New<String>("TA_FUNC_UNST_STOCHRSI").ToLocalChecked(), New(TA_FUNC_UNST_STOCHRSI));
    Set(func_object, New<String>("TA_FUNC_UNST_T3").ToLocalChecked(), New(TA_FUNC_UNST_T3));
    Set(func_object, New<String>("TA_FUNC_UNST_ALL").ToLocalChecked(), New(TA_FUNC_UNST_ALL));
    Set(func_object, New<String>("TA_FUNC_UNST_NONE").ToLocalChecked(), New(TA_FUNC_UNST_NONE));

    // Return function IDs
    info.GetReturnValue().Set(func_object);
}

NAN_METHOD(SetUnstablePeriod) {

    // Check if any arguments are passed
    if (info.Length() < 2) {
        ThrowTypeError("Two arguments required - FunctionUnstId, unstablePeriod");
        return;
    }

    // Check if first parameter is a string
    if (!info[0]->IsNumber()) {
        ThrowTypeError("First argument must be an Integer");
        return;
    }

    // Check if first parameter is a string
    if (!info[1]->IsNumber()) {
        ThrowTypeError("Second argument must be an Integer");
        return;
    }

    // Retreive the parameters
    TA_FuncUnstId func_id = (TA_FuncUnstId)info[0]->Uint32Value();
    int unstable_period = info[1]->Uint32Value();
    
    info.GetReturnValue().Set(false);
    if (TA_SetUnstablePeriod(func_id, unstable_period) == TA_SUCCESS) {
        info.GetReturnValue().Set(true);
    }
}

class ExecuteWorker : public AsyncWorker {
 public:
  ExecuteWorker(Callback *callback, work_object *wo): AsyncWorker(callback), wo(wo) {}
  ~ExecuteWorker() {

    // Clear the arrays allocated
    for (int i=0; i < wo->garbage_count; i++) {
        delete[] wo->garbage[i];
    }
    delete[] wo->garbage;

    // Clear parameter holder memory
    TA_ParamHolderFree(wo->func_params);
    
    // Dispose output arrays
    for (int i=0; i < wo->nbOutput; i++) {
        delete wo->outReal[i];
        delete wo->outInt[i];
    }
    delete wo->outReal;
    delete wo->outInt;
    
    // Dispose work object
    delete wo;

  }

  void Execute () {

    // Execute the function call with parameters declared
    wo->retCode = TA_CallFunc((const TA_ParamHolder *)wo->func_params, wo->startIdx, wo->endIdx, &wo->outBegIdx, &wo->outNBElement);

  }

  void HandleOKCallback () {
    HandleScope scope;

    // Create the outputs object
    Local<Object> outputArray = New<Object>();
    
    // Execution result object 
    Local<Object> result = New<Object>();
    
    // Result info
    Local<Value> argv[2];
    
    // Determine the number of results
    int resultLength = wo->outNBElement;
    
    // Function output parameter information
    const TA_OutputParameterInfo *output_paraminfo;
    
    // Check for execution error
    if (wo->retCode != TA_SUCCESS) {
        return REPORT_TA_ERROR(callback, wo->retCode);
    }
    
    // Set beginning index and number of elements
    Set(result, New<String>("begIndex").ToLocalChecked(), New<Number>(wo->outBegIdx));
    Set(result, New<String>("nbElement").ToLocalChecked(), New<Number>(wo->outNBElement));
    
    // Loop for all the output parameters
    for (int i=0; i < wo->nbOutput; i++) {
        
        // Get the output parameter information
        TA_GetOutputParameterInfo(wo->func_handle, i, &output_paraminfo);
        
        // Create an array for results
        Local<Array> resultArray = New<Array>(resultLength);
        
        // Loop for all the results
        for (int x = 0; x < resultLength; x++) {
            
            // Determine the output type
            switch(output_paraminfo->type) {
                    
                    // Output type real is needed
                case TA_Output_Real:
                    
                    // Set the real output value
                    Set(resultArray, x, New<Number>(wo->outReal[i][x]));
                    
                    break;
                    
                    // Output type integer is needed
                case TA_Output_Integer:
                    
                    // Set the integer output value
                    Set(resultArray, x, New<Number>(wo->outInt[i][x]));
                    
                    break;
            }
            
        }
        
        // Set the result array
        Set(outputArray, New<String>(output_paraminfo->paramName).ToLocalChecked(), resultArray);
        
    }
    
    // Set the outputs array
    Set(result, New<String>("result").ToLocalChecked(), outputArray);
    
    // Return the execution result
    argv[0] = Nan::Null();
    argv[1] = result;
    callback->Call(2, argv);

  };

 private:
  work_object *wo;
};

NAN_METHOD(Execute) {

    // Execution parameter
    Local<Object> executeParameter;

    // Callback function
    Callback *cb;

    // Price values
    double *open            = NULL;
    double *close           = NULL;
    double *high            = NULL;
    double *low             = NULL;
    double *volume          = NULL;
    double *openInterest    = NULL;

    double *inRealList      = NULL;
    
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
    if (info.Length() < 2) {
        ThrowTypeError("Two arguments required - Object and Function");
        return;
    }

    // Check the execution parameter
    if (!info[0]->IsObject()) {
        ThrowTypeError("First argument must be an Object with fields name, startIdx, endIdx and function input parameters");
        return;
    }
    
    // Check the callback parameter
    if (!info[1]->IsFunction()) {
        ThrowTypeError("Second argument must be a Function");
        return;
    }
    
    // Get the execute parameter
    executeParameter = info[0]->ToObject();
    
    // Get the callback function
    cb = new Callback(info[1].As<Function>());
    
    // Check the function name parameter
    if (!HasOwnProperty(executeParameter, New<String>("name").ToLocalChecked()).FromJust()) {
        REPORT_INTERNAL_ERROR(cb, "First argument must contain 'name' field");
        return;
    }
    
    // Retreive the function name string
    Utf8String func_name( Get(executeParameter, New<String>("name").ToLocalChecked()).ToLocalChecked()->ToString() );
    
    // Check the start index
    if (!HasOwnProperty(executeParameter, New<String>("startIdx").ToLocalChecked()).FromJust()) {
        REPORT_INTERNAL_ERROR(cb, "First argument must contain 'startIdx' field");
        return;
    }
    
    // Check the end index
    if (!HasOwnProperty(executeParameter, New<String>("endIdx").ToLocalChecked()).FromJust()) {
        REPORT_INTERNAL_ERROR(cb, "First argument must contain 'endIdx' field");
        return;
    }
    
    // Refreive the start and end index
    int startIdx = Get(executeParameter, New<String>("startIdx").ToLocalChecked()).ToLocalChecked()->Int32Value();
    int endIdx = Get(executeParameter, New<String>("endIdx").ToLocalChecked()).ToLocalChecked()->Int32Value();

    // Check for negative indexes
    if ((startIdx < 0) || (endIdx < 0)) {
        REPORT_INTERNAL_ERROR(cb, "Arguments 'startIdx' and 'endIdx' need to be positive");
        return;
    }

    // Check for index correctness
    if (startIdx > endIdx) {
        REPORT_INTERNAL_ERROR(cb, "Argument 'startIdx' needs to be smaller than argument 'endIdx'");
        return;
    }

    // Retreive the function handle for function name
    if ((retCode = TA_GetFuncHandle(*func_name, &func_handle)) != TA_SUCCESS) {
        REPORT_TA_ERROR(cb, retCode);
        return;
    }
    
    // Retreive the function information for the function handle
    if ((retCode = TA_GetFuncInfo(func_handle, &func_info)) != TA_SUCCESS) {
        REPORT_TA_ERROR(cb, retCode);
        return;
    }

    // Allocate parameter holder memory for function handle
    if ((retCode = TA_ParamHolderAlloc(func_handle, &func_params)) != TA_SUCCESS) {
        REPORT_TA_ERROR(cb, retCode);
        return;
    }

    // Loop for all the input parameters
    double **garbage = (double **)malloc(func_info->nbInput * 6 * sizeof(double*));
    int garbage_count = 0;
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
                    if (!HasOwnProperty(executeParameter, New<String>("open").ToLocalChecked()).FromJust()) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);

                        // Clear values memory
                        delete[] garbage;
                        
                        // Return internal error
                        REPORT_INTERNAL_ERROR(cb, "First argument must contain 'open' field");
                        return;
                        
                    }
                    
                    // Get the open prices
                    open = V8_TO_DOUBLE_ARRAY(Local<Array>::Cast(Get(executeParameter, New<String>("open").ToLocalChecked()).ToLocalChecked()));
                    garbage[garbage_count++] = open;
                }
                
                // Check if the parameter info requires high price
                if (input_paraminfo->flags & TA_IN_PRICE_HIGH) {
                    
                    // Check if the price object has high prices
                    if (!HasOwnProperty(executeParameter, New<String>("high").ToLocalChecked()).FromJust()) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);

                        // Clear values memory
                        delete[] garbage;
                        if (open)
                            delete[] open;
                        
                        // Return internal error
                        REPORT_INTERNAL_ERROR(cb, "First argument must contain 'high' field");
                        return;
                        
                    }
                    
                    // Get the high prices
                    high = V8_TO_DOUBLE_ARRAY(Local<Array>::Cast(Get(executeParameter, New<String>("high").ToLocalChecked()).ToLocalChecked()));
                    garbage[garbage_count++] = high;

                }
                
                // Check if the parameter info requires low price
                if (input_paraminfo->flags & TA_IN_PRICE_LOW) {
                    
                    // Check if the price object has low prices
                    if (!HasOwnProperty(executeParameter, New<String>("low").ToLocalChecked()).FromJust()) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);
                        
                        // Clear values memory
                        delete[] garbage;
                        if (open)
                            delete[] open;
                        if (high)
                            delete[] high;
                        
                        // Return internal error
                        REPORT_INTERNAL_ERROR(cb, "First argument must contain 'low' field");
                        return;
                        
                    }
                    
                    // Get the low prices
                    low = V8_TO_DOUBLE_ARRAY(Local<Array>::Cast(Get(executeParameter, New<String>("low").ToLocalChecked()).ToLocalChecked()));
                    garbage[garbage_count++] = low;
                    
                }
                
                // Check if the parameter info requires close price
                if (input_paraminfo->flags & TA_IN_PRICE_CLOSE) {
                    
                    // Check if the price object has close prices
                    if (!HasOwnProperty(executeParameter, New<String>("close").ToLocalChecked()).FromJust()) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);
                        
                        // Clear values memory
                        delete[] garbage;
                        if (open)
                            delete[] open;
                        if (low)
                            delete[] low;
                        if (high)
                            delete[] high;
                        
                        // Return internal error
                        REPORT_INTERNAL_ERROR(cb, "First argument must contain 'close' field");
                        return;
                        
                    }
                    
                    // Get the close prices
                    close = V8_TO_DOUBLE_ARRAY(Local<Array>::Cast(Get(executeParameter, New<String>("close").ToLocalChecked()).ToLocalChecked()));
                    garbage[garbage_count++] = close;
                    
                }
                
                // Check if the parameter info requires volume
                if (input_paraminfo->flags & TA_IN_PRICE_VOLUME) {
                    
                    // Check if the price object has volume
                    if (!HasOwnProperty(executeParameter, New<String>("volume").ToLocalChecked()).FromJust()) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);

                        // Clear values memory
                        delete[] garbage;
                        if (open)
                            delete[] open;
                        if (close)
                            delete[] close;
                        if (low)
                            delete[] low;
                        if (high)
                            delete[] high;
                        
                        // Return internal error
                        REPORT_INTERNAL_ERROR(cb, "First argument must contain 'volume' field");
                        return;
                        
                    }
                    
                    // Get the volume
                    volume = V8_TO_DOUBLE_ARRAY(Local<Array>::Cast(Get(executeParameter, New<String>("volume").ToLocalChecked()).ToLocalChecked()));
                    garbage[garbage_count++] = volume;
                    
                }
                
                // Check if the parameter info requires open interest values
                if (input_paraminfo->flags & TA_IN_PRICE_OPENINTEREST) {
                    
                    // Check if the price object has open interest values
                    if (!HasOwnProperty(executeParameter, New<String>("openInterest").ToLocalChecked()).FromJust()) {
                        
                        // Clear parameter holder memory
                        TA_ParamHolderFree(func_params);
                        
                        // Clear values memory
                        delete[] garbage;
                        if (close)
                            delete[] close;
                        if (volume)
                            delete[] volume;
                        if (high)
                            delete[] high;
                        if (low)
                            delete[] low;
                        
                        // Return internal error
                        REPORT_INTERNAL_ERROR(cb, "First argument must contain 'openInterest' field");
                        return;
                        
                    }
                    
                    // Get the open interest values
                    openInterest = V8_TO_DOUBLE_ARRAY(Local<Array>::Cast(Get(executeParameter, New<String>("openInterest").ToLocalChecked()).ToLocalChecked()));
                    garbage[garbage_count++] = openInterest;
                    
                }
                
                // Save the price parameters
                if ((retCode = TA_SetInputParamPricePtr(func_params, i, open, high, low, close, volume, openInterest)) != TA_SUCCESS) {
                    
                    // Clear parameter holder memory
                    TA_ParamHolderFree(func_params);
                    
                    // Return TA error
                    REPORT_TA_ERROR(cb, retCode);
                    return;
                     
                }
                
                break;

            // Input parameter real type is needed
            case TA_Input_Real:
                
                // Check if the input parameter object has real value
                if (!HasOwnProperty(executeParameter, New<String>(input_paraminfo->paramName).ToLocalChecked()).FromJust()) {
                    
                    // Clear parameter holder memory
                    TA_ParamHolderFree(func_params);
                    
                    // Return internal error
                    REPORT_INTERNAL_ERROR(cb, ((std::string)("First argument must contain '") + (std::string)(input_paraminfo->paramName) + (std::string)("' field")).c_str());
                    return;
                    
                }
                
                // Get the number parameter value
                inRealList = V8_TO_DOUBLE_ARRAY(Local<Array>::Cast(Get(executeParameter, New<String>(input_paraminfo->paramName).ToLocalChecked()).ToLocalChecked()));
                garbage[garbage_count++] = inRealList;
                 
                // Save the number parameter
                if ((retCode = TA_SetInputParamRealPtr(func_params, i, inRealList)) != TA_SUCCESS) {
                    
                    // Clear parameter holder memory
                    TA_ParamHolderFree(func_params);
                    
                    // Return TA error
                    REPORT_TA_ERROR(cb, retCode);
                    return;
                    
                }
                
                break;
            
            // Input parameter integer type is needed
            case TA_Input_Integer:
                
                // Check if the input parameter object has integer parameter
                if (!HasOwnProperty(executeParameter, New<String>(input_paraminfo->paramName).ToLocalChecked()).FromJust()) {
                    
                    // Clear parameter holder memory
                    TA_ParamHolderFree(func_params);
                    
                    // Return internal error
                    REPORT_INTERNAL_ERROR(cb, ((std::string)("First argument must contain '") + (std::string)(input_paraminfo->paramName) + (std::string)("' field")).c_str());
                    return;

                }
                
                // Get the integer parameter value
                inInteger = Get(executeParameter, New<String>(input_paraminfo->paramName).ToLocalChecked()).ToLocalChecked()->IntegerValue();
                
                // Save the integer parameter
                if ((retCode = TA_SetInputParamIntegerPtr(func_params, i, &inInteger)) != TA_SUCCESS) {
                    
                    // Clear parameter holder memory
                    TA_ParamHolderFree(func_params);
                    
                    // Return TA error
                    REPORT_TA_ERROR(cb, retCode);
                    return;
                    
                }
                
                break;

        }

    }

    // Loop for all the optional input parameters
    for (int i=0; i < (int)func_info->nbOptInput; i++) {
        
        // Get the optional input parameter information
        TA_GetOptInputParameterInfo(func_info->handle, i, &opt_paraminfo);
        
        // Check if the optional parameter object has real value
        if (!HasOwnProperty(executeParameter, New<String>(opt_paraminfo->paramName).ToLocalChecked()).FromJust()) {
            
            // Clear parameter holder memory
            TA_ParamHolderFree(func_params);
            
            // Return internal error
            REPORT_INTERNAL_ERROR(cb, ((std::string)("First argument must contain '") + (std::string)(opt_paraminfo->paramName) + (std::string)("' field")).c_str());
            return;

        }
        
        // Check the optional input parameter type
        switch (opt_paraminfo->type) {
                
            case TA_OptInput_RealRange:
            case TA_OptInput_RealList:
                
                // Get the integer parameter value
                inReal = Get(executeParameter, New<String>(opt_paraminfo->paramName).ToLocalChecked()).ToLocalChecked()->NumberValue();
                
                // Save the integer parameter
                if ((retCode = TA_SetOptInputParamReal(func_params, i, inReal)) != TA_SUCCESS) {
                    
                    // Clear parameter holder memory
                    TA_ParamHolderFree(func_params);
                    
                    // Return TA error
                    REPORT_TA_ERROR(cb, retCode);
                    return;
                    
                }
                
                break;
            case TA_OptInput_IntegerRange:
            case TA_OptInput_IntegerList:
                
                // Get the integer parameter value
                inInteger = Get(executeParameter, New<String>(opt_paraminfo->paramName).ToLocalChecked()).ToLocalChecked()->IntegerValue();
                
                // Save the integer parameter
                if ((retCode = TA_SetOptInputParamInteger(func_params, i, inInteger)) != TA_SUCCESS) {
                    
                    // Clear parameter holder memory
                    TA_ParamHolderFree(func_params);
                    
                    // Return TA error
                    REPORT_TA_ERROR(cb, retCode);
                    return;
                    
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
    wo->garbage = garbage;
    wo->garbage_count = garbage_count;
    
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

    // Queue the work
    AsyncQueueWorker(new ExecuteWorker(cb, wo));
    return;
}

void Init(Handle<Object> exports, Handle<Object> module) {

    // Initialize the engine
    TA_Initialize();

    // Define fields
    Set(exports, New<String>("version").ToLocalChecked(), New<String>("1.0.3").ToLocalChecked());

    // Define accessors
    SetAccessor(exports, New<String>("functions").ToLocalChecked(), Functions);
    SetAccessor(exports, New<String>("functionUnstIds").ToLocalChecked(), FunctionUnstIds);

    // Define functions
    Set(exports, New<String>("explain").ToLocalChecked(), GetFunction(New<FunctionTemplate>(Explain)).ToLocalChecked());
    Set(exports, New<String>("execute").ToLocalChecked(), GetFunction(New<FunctionTemplate>(Execute)).ToLocalChecked());
    Set(exports, New<String>("setUnstablePeriod").ToLocalChecked(), GetFunction(New<FunctionTemplate>(SetUnstablePeriod)).ToLocalChecked());
}

NODE_MODULE(talib, Init)
