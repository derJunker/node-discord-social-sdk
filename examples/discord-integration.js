// Example: Discord SDK Integration with Rich Presence and Authorization
const { DiscordSocialSDK } = require('../dist/index.js');

// Replace with your Discord application's client ID
const CLIENT_ID = "1434167411791368233";

console.log(`type of client ID: ${typeof CLIENT_ID}`);

const sdk = new DiscordSocialSDK();

console.log('Initializing Discord SDK...');
const initialized = sdk.initialize(CLIENT_ID);

if (!initialized) {
    console.error('Failed to initialize Discord SDK.');
    console.error('Make sure:');
    console.error('1. The Discord Social SDK is installed in lib/discord_social_sdk/');
    console.error('2. Your client ID is correct');
    console.error('3. Discord is running on your system');
    process.exit(1);
}

console.log('✓ Discord SDK initialized successfully!\n');

// Example 2: Authorize User
console.log('Example 2: Authorizing User');
sdk.authorize((result) => {
    if (result.success) {
        console.log('✓ Authorization successful!');
        console.log('  Access Token:', result.accessToken.substr(0, 20) + '...\n');
    } else {
        console.error('✗ Authorization failed:', result.error, '\n');
    }
});

// // Example 1: Update Rich Presence
// console.log('Example 1: Updating Rich Presence');
// try {
//     sdk.updateActivity({
//         state: 'In Menu',
//         details: 'Browsing Games',
//         startTimestamp: Date.now(),
//         partyId: 'party_' + Math.random().toString(36).substr(2, 9),
//         partySize: 1,
//         partyMax: 4
//     });
//     console.log('✓ Rich presence updated\n');
// } catch (error) {
//     console.error('✗ Failed to update activity:', error.message, '\n');
// }

// // Run callbacks to process SDK events
// let callbackCount = 0;
// const callbackInterval = setInterval(() => {
//     sdk.runCallbacks();
//     callbackCount++;
    
//     // Run for 5 seconds, then demonstrate clearing activity
//     if (callbackCount === 50) {
//         console.log('Example 3: Clearing Rich Presence');
//         try {
//             sdk.clearActivity();
//             console.log('✓ Rich presence cleared\n');
//         } catch (error) {
//             console.error('✗ Failed to clear activity:', error.message, '\n');
//         }
//     }
    
//     // Exit after 10 seconds
//     if (callbackCount >= 100) {
//         clearInterval(callbackInterval);
        
//         console.log('Shutting down Discord SDK...');
//         sdk.shutdown();
//         console.log('✓ SDK shutdown complete');
        
//         process.exit(0);
//     }
// }, 100);

// Handle cleanup on exit
process.on('SIGINT', () => {
    console.log('\nReceived SIGINT, shutting down...');
    clearInterval(callbackInterval);
    sdk.shutdown();
    process.exit(0);
});
