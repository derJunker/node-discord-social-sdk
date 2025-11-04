const { DiscordSocialSDK } = require('../dist/index.js');

console.log('Testing Discord Social SDK...\n');

try {
    // Create an instance
    const sdk = new DiscordSocialSDK();
    console.log('✓ SDK instance created successfully');

    // Test getUserInfo
    const userId = 'user123';
    const userInfo = sdk.getUserInfo(userId);
    console.log(`✓ getUserInfo("${userId}"): ${userInfo}`);

    // Test sendMessage
    const channelId = 'channel456';
    const message = 'Hello from Node.js!';
    const result = sdk.sendMessage(channelId, message);
    console.log(`✓ sendMessage("${channelId}", "${message}"): ${result}`);

    // Test getOnlineUsers
    const onlineUsers = sdk.getOnlineUsers();
    console.log(`✓ getOnlineUsers(): ${onlineUsers}`);

    console.log('\n✅ All tests passed!');
} catch (error) {
    console.error('❌ Test failed:', error.message);
    process.exit(1);
}
