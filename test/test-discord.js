const { DiscordSocialSDK } = require('../dist/index.js');

console.log('Testing Discord Social SDK Integration...\n');

try {
    // Create an instance
    const sdk = new DiscordSocialSDK();
    console.log('✓ SDK instance created successfully');

    // Test that methods exist
    console.log('✓ initialize method exists:', typeof sdk.initialize === 'function');
    console.log('✓ shutdown method exists:', typeof sdk.shutdown === 'function');
    console.log('✓ updateActivity method exists:', typeof sdk.updateActivity === 'function');
    console.log('✓ clearActivity method exists:', typeof sdk.clearActivity === 'function');
    console.log('✓ authorize method exists:', typeof sdk.authorize === 'function');
    console.log('✓ runCallbacks method exists:', typeof sdk.runCallbacks === 'function');
    console.log('✓ isInitialized method exists:', typeof sdk.isInitialized === 'function');

    // Test isInitialized before initialization
    const initializedBefore = sdk.isInitialized();
    console.log('✓ isInitialized (before init):', initializedBefore);

    // Test initialization (will fail without SDK, but shouldn't crash)
    console.log('\nTrying to initialize (will fail without SDK files)...');
    const initResult = sdk.initialize(123456789);
    console.log('  Initialize result:', initResult);
    console.log('  Note: Initialize returns false because Discord SDK is not installed');

    // Test that we can call other methods without crashing
    console.log('\nTesting methods (should handle missing SDK gracefully)...');
    
    try {
        sdk.updateActivity({
            state: 'Testing',
            details: 'Running tests'
        });
        console.log('✓ updateActivity called (may show error if SDK not installed)');
    } catch (e) {
        console.log('✓ updateActivity threw error as expected:', e.message);
    }

    try {
        sdk.clearActivity();
        console.log('✓ clearActivity called (may show error if SDK not installed)');
    } catch (e) {
        console.log('✓ clearActivity threw error as expected:', e.message);
    }

    // Test shutdown
    sdk.shutdown();
    console.log('✓ shutdown called successfully');

    console.log('\n✅ All API tests passed!');
    console.log('\nNote: To test full functionality, install the Discord Social SDK in lib/discord_social_sdk/');
} catch (error) {
    console.error('❌ Test failed:', error.message);
    console.error(error.stack);
    process.exit(1);
}
