//
//  ViewController.m
//  Thesis
//
//  Created by Quincy Bock on 4/6/14.
//  Copyright (c) 2014 Quincy Bock. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    NSString *fullURL = @"http://54.235.78.70/name/index.php";
    NSURL *url = [NSURL URLWithString:fullURL];
    NSURLRequest *requestObj = [NSURLRequest requestWithURL:url];
    [_viewWeb loadRequest:requestObj];
//    [_viewWeb reload];
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


- (void)willEnterForeground {
    [_viewWeb reload];
}

- (BOOL)prefersStatusBarHidden {
    return YES;
}

@end
