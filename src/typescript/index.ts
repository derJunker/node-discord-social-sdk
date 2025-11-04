import * as path from 'path';

// Interface for the native addon
interface NativeDiscordSDK {
    getUserInfo(userId: string): string;
    sendMessage(channelId: string, message: string): boolean;
    getOnlineUsers(): number;
}

// Interface for the addon constructor
interface NativeAddon {
    DiscordSocialSDK: new () => NativeDiscordSDK;
}

// Load the native addon with flexible path resolution
function loadNativeAddon(): NativeAddon {
    // Try Release build first, fallback to Debug
    const buildConfigs = ['Release', 'Debug'];
    
    for (const config of buildConfigs) {
        try {
            const addonPath = path.join(__dirname, `../build/${config}/discord_social_sdk.node`);
            return require(addonPath);
        } catch (error) {
            // Try next configuration
        }
    }
    
    // If no specific build found, try default path
    try {
        const addonPath = path.join(__dirname, '../build/discord_social_sdk.node');
        return require(addonPath);
    } catch (error) {
        throw new Error(
            'Failed to load native addon. Please ensure the project is built using "npm run build" or "npm install".'
        );
    }
}

const addon = loadNativeAddon();

/**
 * Discord Social SDK - TypeScript wrapper for native C++ module
 */
export class DiscordSocialSDK {
    private nativeInstance: NativeDiscordSDK;

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
