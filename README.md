# C.R.O.S. The Cubic Robitics Operating System

Welcome to the CROS. My pretty poor attempt at creating a robotics OS for use with my accompanying Reaction wheel balanced Cube. The required Library, the SensorCaller (name subject to change) is also included in the repo.

The CROS allows access via serial to the Benture Box main interface. Currnetly using SimpleCLI a user can access all sensors individually and see any values required. The user can adjust individual controller gains, turn them on or off and also plot entire subsystem data. It's a basic interface but is an excellent starting point for anyone looking to use MADGWICK filters for angular display and control. I will likely be upgrading the CLI to another at somepoint, or changing to another core as the OS isn't responsive at times likely due to TaskScheduler. 
