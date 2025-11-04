import { DiscordSocialSDK } from '../dist/index';

console.log('Testing Discord Social SDK with TypeScript...\n');

try {
    // Create an instance
    const sdk: DiscordSocialSDK = new DiscordSocialSDK();
    console.log('✓ SDK instance created successfully');

    // Test getUserInfo - should return string
    const userId: string = 'user123';
    const userInfo: string = sdk.getUserInfo(userId);
    console.log(`✓ getUserInfo("${userId}"): ${userInfo}`);

    // Test sendMessage - should return boolean
    const channelId: string = 'channel456';
    const message: string = 'Hello from TypeScript!';
    const result: boolean = sdk.sendMessage(channelId, message);
    console.log(`✓ sendMessage("${channelId}", "${message}"): ${result}`);

    // Test getOnlineUsers - should return number
    const onlineUsers: number = sdk.getOnlineUsers();
    console.log(`✓ getOnlineUsers(): ${onlineUsers}`);

    console.log('\n✅ All TypeScript tests passed!');
} catch (error: any) {
    console.error('❌ Test failed:', error.message);
    process.exit(1);
}
