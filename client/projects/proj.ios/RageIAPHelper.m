//
//  RageIAPHelper.m
//  In App Rage
//
//  Created by Ray Wenderlich on 9/5/12.
//  Copyright (c) 2012 Razeware LLC. All rights reserved.
//

#import "RageIAPHelper.h"

@implementation RageIAPHelper

+ (RageIAPHelper *)sharedInstance {
    static dispatch_once_t once;
    static RageIAPHelper * sharedInstance;
    dispatch_once(&once, ^{
        NSSet * productIdentifiers = [NSSet setWithObjects:
                                      @"com.qfkj.crazyass.peach5",
                                      @"com.qfkj.crazyass.peach30",
                                      @"com.qfkj.crazyass.peach60",
                                      @"com.qfkj.crazyass.peach200",
                                      @"com.qfkj.crazyass.peach700",
                                      nil];
        sharedInstance = [[self alloc] initWithProductIdentifiers:productIdentifiers];
    });
    NSLog(@"shared RageIAPHelper %p\n", sharedInstance);
    return sharedInstance;
}

@end
