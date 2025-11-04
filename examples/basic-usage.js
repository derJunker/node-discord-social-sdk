// Example: Basic usage with JavaScript
const { DiscordSocialSDK } = require('../dist/index.js');

// Create an instance of the SDK
const sdk = new DiscordSocialSDK();

// Example 1: Get user information
console.log('Example 1: Get User Info');
const userInfo = sdk.getUserInfo('user123');
console.log(`Result: ${userInfo}\n`);

// Example 2: Send a message
console.log('Example 2: Send Message');
const success = sdk.sendMessage('channel456', 'Hello World!');
console.log(`Message sent successfully: ${success}\n`);

// Example 3: Get online users count
console.log('Example 3: Get Online Users');
const onlineUsers = sdk.getOnlineUsers();
console.log(`Online users: ${onlineUsers}\n`);

console.log('✅ All examples completed!');
