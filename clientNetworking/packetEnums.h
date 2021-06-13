#ifndef TICTACTOE_PACKETENUMS_H
#define TICTACTOE_PACKETENUMS_H

enum packetType { ServicePacket = 1, DataRequestPacket = 2, dataSendPacket = 3};

enum servicePacketType {ServicePingRequest = 1, ServicePingResponse = 2, ErrorPacket = 3, ServiceSuccess = 4,
        ServiceUserAction = 5, EndGame = 6, ServiceNotificationPacket = 7};

enum dataRequestPacketType {LoginRequest = 1, RequestProfileData = 2, RequestPlayerList = 3,
    RequestGameHistory = 4, RequestPlayerMove = 5, RequestCheckUsername = 6, RequestCheckUserStatus = 7};

enum dataSendPacketType {SendLoginData = 1, SendRegisterRequest = 2, SendProfileData = 3, SendPlayerList = 4,
    SendGameStateHistory = 5, SendPlayerMove = 6, SendUserStatus = 7};

enum errorCodesEnum {PermissionDeniedErr = 1, UserBlacklistedErr = 2, UserBusyErr = 3,
    LoginDeniedErr = 4, NoRegUsernameTakenErr = 5, NoRegBadFormat = 6, BadMoveErr = 7,
    LoginRequiiredErr = 8, BadStateErr = 9, BadActionTargetErr = 10,
    BadPacketErr = 11, UserNoExistErr = 12, UserOffline = 13, GameDeclinedErr = 13,
    FriendshipDeclinedErr = 14, ImATeapotErr = 42};

enum userActionsEnum {GameRequestAction = 1, BlacklistAction = 2, FriendRequestAction = 3,
    AccFriendRequestAction = 4, LogOutAction = 5, MakeProfilePrivateAction = 6,
    MakeProfilePublicAction = 7, FriendRequestAcceptAction = 8, DeclineFriendRequestAction = 9,
    GameAcceptAction = 10, GameDeclineAction = 11, Disconnect = 12};

#endif //TICTACTOE_PACKETENUMS_H
