ADD_SERVICE("feed/feedview" "feedview" BaseFeed DbpoolFeed OceSliceRFeed OceSliceILikeIt ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver OceCxxAdapterFeedStatLogAdapter OceCxxAdapterFeedFocusAdapter OceCxxAdapterFeedNewsReplicaAdapter OceCxxAdapterFeedMiniReplicaAdapter OceCxxAdapterControllerAdapter OceCxxAdapterVipIconDataReaderAdapter OceCxxAdapterILikeItCacheAdapter IceExt  OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice ctemplate NotifyUtil)

#ADD_SERVICE_EXEC("feed/cwf_feed" "cwf" BaseFeed DbCxxPool ctemplate OceSliceRFeed OceSliceILikeIt  ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver OceCxxAdapterFeedClassAdapter  OceCxxAdapterFeedNewsReplicaAdapter OceCxxAdapterFeedMiniReplicaAdapter  OceCxxAdapterControllerAdapter OceCxxAdapterVipIconDataReaderAdapter OceCxxAdapterILikeItCacheAdapter OceSliceUserCache OceSliceTicket IceExt OceSlice UtilCxx UtilSlice Fcgi NotifyUtil)

#ADD_SERVICE("feed/FeedCache/src" "FeedCache" OceCxxAdapterFeedDBAdapter OceCxxAdapterSSDFeedDBAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt  OceSliceXceStorm DbCxxPool UtilCxx UtilSlice breakpad OceSlice)
ADD_SERVICE("feed/FeedCacheN/src" "FeedCacheN" OceCxxAdapterFeedCacheLogicAdapter OceCxxAdapterFeedDBAdapter OceCxxAdapterSSDFeedDBAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt  OceSliceXceStorm OceCxxAdapterFeedCacheLogicReplicaAdapter DbCxxPool UtilCxx UtilSlice breakpad OceSlice)
ADD_SERVICE("feed/FeedCacheLogic/src" "FeedCacheLogic" OceCxxAdapterFeedDBTAdapter OceCxxAdapterSSDFeedDBTAdapter OceCxxAdapterSSDFeedDBAdapter OceCxxAdapterFeedNewsAdapter OceCxxAdapterFeedCacheNReplicaAdapter OceCxxAdapterFeedCacheNAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt  OceSliceXceStorm DbCxxPool FeedMemcClient UtilCxx UtilSlice breakpad OceSlice)


ADD_SERVICE("feed/FeedFocus/src" "FeedFocus" OceCxxAdapterFeedItemCacheByUseridReplicaAdapter OceCxxAdapterFeedUserRecordAdapter OceCxxAdapterFeedMiniAdapter OceCxxAdapterFeedContentAdapter OceCxxAdapterFeedAssistantNAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt  OceSliceXceStorm DbCxxPool UtilCxx UtilSlice breakpad FeedMemcClient OceSlice)
ADD_SERVICE("feed/FeedAssistantN/src" "FeedAssistantN" OceCxxAdapterPhotoDataGatheringAdapter OceCxxAdapterFeedItemCacheAdapter OceCxxAdapterUserStateAdapter OceCxxAdapterFeedGroupAdapter OceCxxAdapterFeedSchoolAdapter OceCxxAdapterUserNetworkAdapter OceCxxAdapterFeedNewsAdapter OceCxxAdapterFeedInteractionNAdapter OceCxxAdapterFeedAssistantAdapter OceCxxAdapterFeedAssistantNAdapter OceCxxAdapterFeedDispatcherAdapter OceCxxAdapterFeedMiniAdapter OceCxxAdapterFeedCacheAdapter OceCxxAdapterFeedContentAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt  OceSliceXceStorm DbCxxPool UtilCxx UtilSlice FeedMemcClient OceSlice TalkUtil)

ADD_SERVICE("feed/FeedAssistant/src" "FeedAssistant" OceCxxAdapterFeedItemFacadeReplicaAdapter OceCxxAdapterFeedCacheLogicAdapter OceCxxAdapterFeedCacheLogicReplicaAdapter OceCxxAdapterFeedInteractionNAdapter OceCxxAdapterFeedContentAdapter OceCxxAdapterFeedAssistantNReplicaAdapter OceCxxAdapterFeedMiniAdapter OceCxxAdapterFeedAssistantNAdapter OceCxxAdapterUserDangerAdapter OceCxxAdapterUserPassportAdapter OceCxxAdapterFeedStatAdapter OceCxxAdapterFeedNewsAdapter OceCxxAdapterFeedDispatcherAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt  OceSliceXceStorm DbCxxPool UtilCxx UtilSlice FeedMemcClient OceSlice TalkUtil )


ADD_SERVICE("feed/FeedDispatcherN/src" "FeedDispatcherN" OceCxxAdapterFeedSinkAdapter OceCxxAdapterFeedCacheLogicAdapter OceCxxAdapterFeedNaiveBayesReplicaAdapter OceCxxAdapterUserStateAdapter OceCxxAdapterUserConfigAdapter OceCxxAdapterPhotoDataGatheringAdapter OceCxxAdapterFeedNewsReplicaAdapter OceCxxAdapterFeedInteractionNAdapter OceSliceFeedConfig OceCxxAdapterFeedConfigAdapter   ReplicaCluster OceCxxAdapterFeedDBTAdapter OceCxxAdapterSSDFeedDBAdapter OceCxxAdapterFriendRankCacheAdapter OceCxxAdapterWTransitAdapter OceCxxAdapterBuddyByIdReplicaAdapter OceCxxAdapterFeedMiniReplicaAdapter OceCxxAdapterFeedCacheAdapter OceCxxAdapterFeedDBAdapter IceExt OceSlice OceSliceXceStorm DbCxxPool FeedMemcClient  TalkUtil UtilCxx UtilSlice mysqlpp)
ADD_SERVICE("feed/FeedMini2/src" "FeedMini2" OceCxxAdapterFeedCacheAdapter OceCxxAdapterFeedContentAdapter OceCxxAdapterFeedAssistantNAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt  OceSliceXceStorm DbCxxPool UtilCxx UtilSlice FeedMemcClient OceSlice)
#ADD_SERVICE("feed/FeedMemcProxy/proxy" "FeedMemcProxy" OceCxxAdapterFeedMemcLoaderAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt  OceSliceXceStorm DbCxxPool UtilCxx UtilSlice OceSlice memcached protobuf boost_system)
#ADD_SERVICE("feed/FeedMemcProxy/loader" "FeedMemcLoader" ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt  OceSliceXceStorm DbCxxPool UtilCxx UtilSlice memcached protobuf OceSlice FeedMemcClient memcached protobuf boost_system)
ADD_SERVICE("feed/FeedMemcProxy/content" "FeedMemcContent" ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt  OceSliceXceStorm DbCxxPool UtilCxx UtilSlice FeedMemcClient OceSlice boost_system-gcc41-mt breakpad)
ADD_SERVICE("feed/FeedSchool/src" "FeedSchool" OceCxxAdapterFeedCacheAdapter OceCxxAdapterFeedContentAdapter OceCxxAdapterFeedAssistantNAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt  OceSliceXceStorm DbCxxPool UtilCxx UtilSlice FeedMemcClient memcached protobuf FeedMemcClient OceSlice memcached protobuf)
ADD_SERVICE("feed/FeedGroup/src" "FeedGroup" OceCxxAdapterFeedCacheAdapter OceCxxAdapterFeedContentAdapter OceCxxAdapterFeedAssistantNAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt  OceSliceXceStorm DbCxxPool UtilCxx UtilSlice FeedMemcClient memcached protobuf FeedMemcClient OceSlice)
#ADD_SERVICE("feed/FeedInteraction/src" "FeedInteraction" OceCxxAdapterFeedInteractionNAdapter ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt  OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice breakpad)
#ADD_SERVICE("feed/FeedInteractionN/src" "FeedInteractionN" ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt  OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice breakpad)
ADD_SERVICE("feed/FeedStat/src" "FeedStat" IceExt OceSlice UtilCxx OceCxxAdapterFeedStatAdapter OceSliceXceStorm UtilSlice DbCxxPool)
#ADD_SERVICE("feed/FeedEdmSender/src" "FeedEdmSender" bjson_linux-gcc-4.1.2_libmt OceCxxAdapterFeedEdmSenderAdapter OceCxxAdapterUserNetworkAdapter OceSliceFeedEdmSender ReplicaCluster IceExt OceSliceReplicaCluster OceSliceXceStorm DbCxxPool  UtilCxx UtilSlice mysqlpp)
#ADD_SERVICE("feed/FeedSyncDb/src" "FeedSyncDb" ReplicaCluster OceSliceReplicaCluster OceSliceSubjectObserver IceExt  OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice breakpad)
ADD_SERVICE("feed/PhotoDataGathering/src" "PhotoDataGathering"  OceSliceReplicaCluster OceSliceSubjectObserver  ReplicaCluster  IceExt  OceSliceXceStorm DbCxxPool OceSlice UtilCxx UtilSlice memcached protobuf FeedMemcClient OceCxxAdapterFeedMemcProxyAdapter memcached protobuf NotifyUtil)
#ADD_SERVICE("feed/SSDFeedDBData/src" "SSDFeedDBData" OceCxxAdapterFeedDBAdapter OceCxxAdapterSSDFeedDBAdapter IceExt OceSlice DbCxxPool UtilCxx UtilSlice aio)
#ADD_SERVICE("feed/FeedDBData/src" "FeedDBData" OceCxxAdapterFeedDBAdapter OceCxxAdapterSSDFeedDBAdapter IceExt OceSlice DbCxxPool UtilCxx UtilSlice aio)
#ADD_SERVICE("feed/FeedDBDataT/src" "FeedDBDataT" OceCxxAdapterFeedDBTAdapter IceExt OceSlice DbCxxPool UtilCxx UtilSlice aio)
#ADD_SERVICE("feed/SSDFeedDBDataT/src" "SSDFeedDBDataT" OceCxxAdapterSSDFeedDBTAdapter IceExt OceSlice DbCxxPool UtilCxx UtilSlice aio)
#ADD_SERVICE("feed/FeedDBMaster/src" "FeedDBMaster" OceCxxAdapterFeedDBAdapter OceCxxAdapterSSDFeedDBAdapter IceExt OceSlice DbCxxPool UtilCxx UtilSlice aio)
#ADD_SERVICE("feed/SSDFeedDBMaster/src" "SSDFeedDBMaster" OceCxxAdapterFeedDBAdapter OceCxxAdapterSSDFeedDBAdapter IceExt OceSlice DbCxxPool UtilCxx UtilSlice tokyocabinet)
#ADD_SERVICE("feed/FeedDBMasterT/src" "FeedDBMasterT" OceCxxAdapterFeedDBTAdapter IceExt OceSlice DbCxxPool UtilCxx UtilSlice tokyocabinet)
#ADD_SERVICE("feed/SSDFeedDBMasterT/src" "SSDFeedDBMasterT" OceCxxAdapterSSDFeedDBTAdapter IceExt OceSlice DbCxxPool UtilCxx UtilSlice tokyocabinet)

ADD_SERVICE("feed/FeedItemCache/src" "FeedItemCacheByUserid" OceCxxAdapterMonitorAdapter ReplicaCluster IceExt OceSlice DbCxxPool UtilCxx UtilSlice)
ADD_SERVICE("feed/FeedItemCacheByAppid/src" "FeedItemCacheByAppid" OceCxxAdapterMonitorAdapter OceCxxAdapterBuddyByOnlineTimeReplicaAdapter ReplicaCluster IceExt OceSlice DbCxxPool UtilCxx UtilSlice)
ADD_SERVICE("feed/FeedItemWrapper/src" "FeedItemWrapper" OceCxxAdapterMonitorAdapter OceCxxAdapterFeedItemCacheAdapter OceCxxAdapterBuddyByOnlineTimeReplicaAdapter ReplicaCluster IceExt OceSlice DbCxxPool UtilCxx UtilSlice)
ADD_SERVICE("feed/FeedItemFacade/src" "FeedItemFacade" OceCxxAdapterMonitorAdapter OceCxxAdapterFeedItemCacheByUseridReplicaAdapter OceCxxAdapterFeedItemCacheByAppidReplicaAdapter ReplicaCluster IceExt OceSlice DbCxxPool UtilCxx UtilSlice)
#ADD_SERVICE_EXEC("feed/FeedItemCache/migrateDB" "FeedItemDBMigrator" OceSlice DbCxxPool IceExt UtilCxx UtilSlice)
ADD_SERVICE_EXEC("feed/FeedItemCacheByAppid/test" "AppidTest" ReplicaCluster OceCxxAdapterFeedItemCacheByAppidReplicaAdapter OceSlice DbCxxPool IceExt UtilCxx UtilSlice)
ADD_SERVICE_EXEC("feed/FeedItemCache/test" "UseridTest" ReplicaCluster OceCxxAdapterFeedItemCacheByUseridReplicaAdapter OceSlice DbCxxPool IceExt UtilCxx UtilSlice)
ADD_SERVICE_EXEC("feed/FeedItemFacade/test" "FacadeTest" ReplicaCluster OceCxxAdapterFeedItemFacadeReplicaAdapter OceSlice DbCxxPool IceExt UtilCxx UtilSlice)