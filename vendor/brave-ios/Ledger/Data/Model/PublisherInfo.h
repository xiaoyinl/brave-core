/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#import <Foundation/Foundation.h>
#import <CoreData/CoreData.h>

@class ActivityInfo, TransactionInfo, RecurringDonation, PendingContribution;

NS_ASSUME_NONNULL_BEGIN

@interface PublisherInfo : NSManagedObject

+ (NSFetchRequest<PublisherInfo *> *)fetchRequest;

@property (nonatomic) int32_t excluded;
@property (nonatomic, copy) NSString *faviconURL;
@property (nonatomic, copy) NSString *name;
@property (nonatomic, copy) NSString *provider;
@property (nonatomic, copy) NSString *publisherID;
@property (nonatomic, copy) NSString *url;
@property (nonatomic) BOOL verified;
@property (nullable, nonatomic, retain) NSSet<ActivityInfo *> *activities;
@property (nullable, nonatomic, retain) NSSet<TransactionInfo *> *transactions;
@property (nullable, nonatomic, retain) NSSet<RecurringDonation *> *recurringDonations;
@property (nullable, nonatomic, retain) NSSet<PendingContribution *> *pendingContributions;

@end

@interface PublisherInfo (CoreDataGeneratedAccessors)

- (void)addActivitiesObject:(ActivityInfo *)value;
- (void)removeActivitiesObject:(ActivityInfo *)value;
- (void)addActivities:(NSSet<ActivityInfo *> *)values;
- (void)removeActivities:(NSSet<ActivityInfo *> *)values;

- (void)addTransactionsObject:(TransactionInfo *)value;
- (void)removeTransactionsObject:(TransactionInfo *)value;
- (void)addTransactions:(NSSet<TransactionInfo *> *)values;
- (void)removeTransactions:(NSSet<TransactionInfo *> *)values;

- (void)addRecurringDonationsObject:(RecurringDonation *)value;
- (void)removeRecurringDonationsObject:(RecurringDonation *)value;
- (void)addRecurringDonations:(NSSet<RecurringDonation *> *)values;
- (void)removeRecurringDonations:(NSSet<RecurringDonation *> *)values;

- (void)addPendingContributionsObject:(PendingContribution *)value;
- (void)removePendingContributionsObject:(PendingContribution *)value;
- (void)addPendingContributions:(NSSet<PendingContribution *> *)values;
- (void)removePendingContributions:(NSSet<PendingContribution *> *)values;

@end

NS_ASSUME_NONNULL_END
