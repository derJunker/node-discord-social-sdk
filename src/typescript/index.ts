import * as path from 'path';

// Load the native addon
const addonPath = path.join(__dirname, '../build/Release/discord_social_sdk.node');
const addon = require(addonPath);

/**
 * Discord Social SDK - TypeScript wrapper for native C++ module
 */
export class DiscordSocialSDK {
    private nativeInstance: any;

    constructor() {
        this.nativeInstance = new addon.DiscordSocialSDK();
    }

    /**
     * Get user information by user ID
     * @param userId - The Discord user ID
     * @returns User information string
     */
    getUserInfo(userId: string): string {
        if (typeof userId !== 'string') {
            throw new TypeError('userId must be a string');
        }
        return this.nativeInstance.getUserInfo(userId);
    }

    /**
     * Send a message to a channel
     * @param channelId - The Discord channel ID
     * @param message - The message to send
     * @returns True if message was sent successfully
     */
    sendMessage(channelId: string, message: string): boolean {
        if (typeof channelId !== 'string' || typeof message !== 'string') {
            throw new TypeError('channelId and message must be strings');
        }
        return this.nativeInstance.sendMessage(channelId, message);
    }

    /**
     * Get the number of online users
     * @returns The count of online users
     */
    getOnlineUsers(): number {
        return this.nativeInstance.getOnlineUsers();
    }
}

// Export the class as default
export default DiscordSocialSDK;
