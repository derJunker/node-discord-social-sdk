import * as path from 'path';

/**
 * Activity data for rich presence
 */
export interface Activity {
    state?: string;
    details?: string;
    largeImageKey?: string;
    largeImageText?: string;
    smallImageKey?: string;
    smallImageText?: string;
    startTimestamp?: number;
    endTimestamp?: number;
    partyId?: string;
    partySize?: number;
    partyMax?: number;
}

/**
 * Authorization result
 */
export interface AuthResult {
    success: boolean;
    accessToken: string;
    error: string;
}

/**
 * Authorization callback type
 */
export type AuthCallback = (result: AuthResult) => void;

// Interface for the native addon
interface NativeDiscordSDK {
    initialize(clientId: number): boolean;
    shutdown(): void;
    updateActivity(activity: Activity): boolean;
    clearActivity(): boolean;
    authorize(scopes: string, callback: AuthCallback): void;
    runCallbacks(): void;
    isInitialized(): boolean;
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
     * Initialize the Discord SDK with your application's client ID
     * @param clientId - Your Discord application's client ID
     * @returns True if initialization was successful
     */
    initialize(clientId: number): boolean {
        if (typeof clientId !== 'number') {
            throw new TypeError('clientId must be a number');
        }
        return this.nativeInstance.initialize(clientId);
    }

    /**
     * Shutdown the Discord SDK
     */
    shutdown(): void {
        this.nativeInstance.shutdown();
    }

    /**
     * Update the rich presence activity
     * @param activity - Activity data to display
     * @returns True if activity was updated successfully
     */
    updateActivity(activity: Activity): boolean {
        if (typeof activity !== 'object') {
            throw new TypeError('activity must be an object');
        }
        return this.nativeInstance.updateActivity(activity);
    }

    /**
     * Clear the current rich presence activity
     * @returns True if activity was cleared successfully
     */
    clearActivity(): boolean {
        return this.nativeInstance.clearActivity();
    }

    /**
     * Authorize the user and get an OAuth2 token
     * @param scopes - OAuth2 scopes to request (space-separated)
     * @param callback - Callback function to receive the authorization result
     */
    authorize(scopes: string, callback: AuthCallback): void {
        if (typeof scopes !== 'string') {
            throw new TypeError('scopes must be a string');
        }
        if (typeof callback !== 'function') {
            throw new TypeError('callback must be a function');
        }
        this.nativeInstance.authorize(scopes, callback);
    }

    /**
     * Run Discord SDK callbacks. This should be called periodically (e.g., in a loop or interval)
     */
    runCallbacks(): void {
        this.nativeInstance.runCallbacks();
    }

    /**
     * Check if the SDK is initialized
     * @returns True if the SDK is initialized
     */
    isInitialized(): boolean {
        return this.nativeInstance.isInitialized();
    }
}

// Export the class as default
export default DiscordSocialSDK;
