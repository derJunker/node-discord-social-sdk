// Example: Basic usage with TypeScript
import { DiscordSocialSDK } from '../dist/index';

// Create an instance of the SDK
const sdk: DiscordSocialSDK = new DiscordSocialSDK();

// Example 1: Get user information
console.log('Example 1: Get User Info');
const userInfo: string = sdk.getUserInfo('user123');
console.log(`Result: ${userInfo}\n`);

// Example 2: Send a message
console.log('Example 2: Send Message');
const success: boolean = sdk.sendMessage('channel456', 'Hello World!');
console.log(`Message sent successfully: ${success}\n`);

// Example 3: Get Online Users count
console.log('Example 3: Get Online Users');
const onlineUsers: number = sdk.getOnlineUsers();
console.log(`Online users: ${onlineUsers}\n`);

console.log('✅ All examples completed!');
